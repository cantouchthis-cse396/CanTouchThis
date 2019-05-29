#include "CommServer.h"

namespace CanTouchThis {

	CommServer::CommServer(unsigned short port, bool start)
			  : lastLocation(0), control(Command::INIT), checkLast(false),
			  	clientCount(MAXIMUM_CLIENTS),
			  	newObj(false)
	{
		acceptor.reset(new boost::asio::ip::tcp::acceptor(ioContext,
			boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port)));
		std::cout << "Can Touch This Communication Server has been initialized.\n";
		if (start == true)
			startTheServer();
	}

	CommServer::~CommServer()
	{
		if (acceptor->is_open())
			acceptor->close();
		for (auto& th : threads)
			th.join();
	}

	void CommServer::acceptClients(void)
	{
		int count = 0;

		while (1) {
			std::shared_ptr<boost::asio::streambuf> buf(new boost::asio::streambuf);
			std::shared_ptr<boost::asio::ip::tcp::socket> sock(
					new boost::asio::ip::tcp::socket(ioContext));
			acceptor->accept(*sock);
			boost::asio::read_until(*sock, *buf, '\n');
			std::istream inputStream(&(*buf));
			std::string clientType;
			std::getline(inputStream, clientType);
			handleClient(clientType, sock, buf);
			++count;
		}
		/* Wait the server until all clients close the connection. */
		std::unique_lock<std::mutex> lock(finishMutex);
		finishCond.wait(lock, [this]{return this->clientCount == 0;});
		std::cout << "The server has been stopped.\n";
	}

	std::string CommServer::streamBufferToString(const boost::asio::streambuf& buf) const
	{
		boost::asio::streambuf::const_buffers_type buffer = buf.data();
		return std::string(boost::asio::buffers_begin(buffer),
						   boost::asio::buffers_begin(buffer) + buf.size() - 1);
	}

	void CommServer::handleClient(const std::string& clientType,
			std::shared_ptr<boost::asio::ip::tcp::socket> sock,
			std::shared_ptr<boost::asio::streambuf> buf)
	{
		std::thread clientThread(([this, clientType, sock, buf]() {
			if (clientType == CLIENT_SCANNER)
				this->scannerServices(sock, buf);
			else if (clientType == CLIENT_CONTROLLER)
				this->controllerServices(sock, buf);
			else if (clientType == CLIENT_VIEWER)
				this->viewerServices(sock, buf);
			else if (clientType == CLIENT_CONVERSION)
				this->conversionServices(sock, buf);
			else
				this->scannerListener(sock, buf);
		}));
		threads.push_back(std::move(clientThread));
	}

	void CommServer::startTheServer(void)
	{
		try {
			acceptor->listen(MAXIMUM_BACKLOG);
			acceptClients();
		} catch (boost::system::system_error& e) {
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	void CommServer::scannerListener(std::shared_ptr<boost::asio::ip::tcp::socket> sock,
									 std::shared_ptr<boost::asio::streambuf> buf)
	{
		try {
			std::string readStr = "";
			std::string comm = "";
			while (true) {
				readStr = readFromSocket(*sock, *buf);
				if (readStr == OP_SCANNER_LISTEN_CONTROLLER) {
					{
						std::unique_lock<std::mutex> lock(controlMutex);
						controlCond.wait(lock, [this]{return this->checkLast;});

						checkLast = false;

						comm = commandToString(this->control);
						std::cout << "Command: " << comm << std::endl;
						boost::property_tree::ptree ptree;
						ptree.put("command", comm);
						std::ostringstream oss;
						boost::property_tree::write_json(oss, ptree);
						boost::asio::write(*sock, boost::asio::buffer(oss.str()));
					}
					//if (comm == CONTROLLER_SHUTDOWN) break;
				} else if (readStr == FAILURE) {
					boost::asio::write(*sock, boost::asio::buffer(FAILURE.c_str(), 3));
				} else if (readStr == END) {
					break;
				}
			}
		} catch (boost::system::system_error& err) {
			std::cerr << err.what() << std::endl;
		}
		control = Command::INIT;
		sock->close();
		// decreaseClientCount();
	}

	void CommServer::scannerServices(std::shared_ptr<boost::asio::ip::tcp::socket> sock,
									 std::shared_ptr<boost::asio::streambuf> buf)
	{
		try {
			std::string readStr;
			while (true) {
				readStr = readFromSocket(*sock, *buf);
				if (readStr == OP_SCANNER_SEND_LOCATION) {
					readStr = lineToJsonString(*sock, *buf);
					boost::property_tree::ptree ptree;
					std::istringstream inputStream(readStr);
					boost::property_tree::read_json(inputStream, ptree);
					{
						double z = ptree.get<double>("z");
						if (z < 0) z = 0;
						std::lock_guard<std::mutex> lock(locationMutex);
						locationData.push_back(std::make_tuple(
								ptree.get<int>("x"), ptree.get<int>("y"), z));
						std::cout << ptree.get<int>("x") << " - "
								  << ptree.get<int>("y") << " - "
								  << z << std::endl;
					}
					locationCond.notify_one();
					boost::asio::write(*sock, boost::asio::buffer(SUCCESS.c_str(), 3));
				} else if (readStr == FAILURE) {
					boost::asio::write(*sock, boost::asio::buffer(FAILURE.c_str(), 3));
				} else if (readStr == END) {
					break;
				}
			}
		} catch (boost::system::system_error& err) {
			std::cerr << err.what() << std::endl;
		}
		sock->close();
		// decreaseClientCount();
		locationData.clear();
		lastLocation = 0;
	}

	void CommServer::decreaseClientCount(void)
	{
		{
			std::lock_guard<std::mutex> lock(finishMutex);
			--clientCount;
		}
		finishCond.notify_one();
	}

	void CommServer::conversionServices(std::shared_ptr<boost::asio::ip::tcp::socket> sock,
									 	std::shared_ptr<boost::asio::streambuf> buf)
	{
		try {
			this->lastLocation = 0;
			std::string readStr;
			while (true) {
				if (control == Command::STOP_SCAN) lastLocation = 0;
				readStr = readFromSocket(*sock, *buf);
				boost::property_tree::ptree ptree;
				if (readStr == OP_CONVERSION_SEND_OBJ) {
					readStr = lineToJsonString(*sock, *buf);
					{
						std::lock_guard<std::mutex> lock(objMutex);
						objFiles.push_back(jsonToObj(readStr));
						newObj = true;
					}
					objCond.notify_one();
					boost::asio::write(*sock, boost::asio::buffer(SUCCESS.c_str(), 3));
				} else if (readStr == OP_CONVERSION_GET_LOC_ONE) {
					if (control == Command::STOP_SCAN) lastLocation = 0;
					{
						std::unique_lock<std::mutex> lock(locationMutex);
						locationCond.wait(lock, [this] {
							return this->lastLocation < this->locationData.size();
						});
						std::tuple<int, int, double> l = locationData[lastLocation];
						std::string value = std::to_string(std::get<0>(l)) + " "
										+ std::to_string(std::get<1>(l)) + " "
										+ std::to_string(std::get<2>(l));
						ptree.put("location", value);
						++lastLocation;
					}
					std::ostringstream oss;
					boost::property_tree::write_json(oss, ptree);
					std::string result = oss.str() + "\n";
					boost::asio::write(*sock, boost::asio::buffer(result));
					if (control == Command::STOP_SCAN) lastLocation = 0;
				} else if (readStr == FAILURE) {
					boost::asio::write(*sock, boost::asio::buffer(FAILURE.c_str(), 3));
				} else if (readStr == END) {
					break;
				}
			}
		} catch (boost::system::system_error& err) {
			std::cerr << err.what() << std::endl;
		}
		sock->close();
		// decreaseClientCount();
	}

	void CommServer::controllerServices(std::shared_ptr<boost::asio::ip::tcp::socket> sock,
									std::shared_ptr<boost::asio::streambuf> buf)
	{
		try {
			std::string readStr;
			while (true) {
				readStr = readFromSocket(*sock, *buf);
				if (readStr == OP_CONTROLLER_COMMAND) {
					readStr = lineToJsonString(*sock, *buf);
					boost::property_tree::ptree ptree;
					std::istringstream iss(readStr);
					boost::property_tree::read_json(iss, ptree);
					std::string command	= ptree.get<std::string>("command");
					{
						// std::lock_guard<std::mutex> lock(controlMutex);
						control = stringToCommand(command);
					}

					{
						std::lock_guard<std::mutex> lock(controlMutex);
						checkLast = true;
					}
					controlCond.notify_one();

					if (control == Command::STOP_SCAN) {
						{
							std::lock_guard<std::mutex> lock(locationMutex);
							locationData.clear();
							lastLocation = 0;
						}
						{
							std::lock_guard<std::mutex> lock(objMutex);
							objFiles.clear();
						}
					}
					boost::asio::write(*sock, boost::asio::buffer(SUCCESS.c_str(), 3));
				} else if (readStr == FAILURE) {
					boost::asio::write(*sock, boost::asio::buffer(FAILURE.c_str(), 3));
				} else if (readStr == END) {
					break;
				}
			}
		} catch (boost::system::system_error& err) {
			std::cerr << err.what() << std::endl;
		}
		sock->close();
		// decreaseClientCount();
	}

	void CommServer::viewerServices(std::shared_ptr<boost::asio::ip::tcp::socket> sock,
									std::shared_ptr<boost::asio::streambuf> buf)
	{
		try {
			std::string readStr;
			while (true) {
				readStr = readFromSocket(*sock, *buf);
				if (readStr == OP_VIEWER_GET_OBJ) {
					{
						std::unique_lock<std::mutex> lock(objMutex);
						objCond.wait(lock, [this]{return newObj == true;});
						boost::asio::write(*sock,
							boost::asio::buffer(objToJson(objFiles.back())));
						newObj = false;
					}
				} else if (readStr == FAILURE) {
					std::cerr << "Might be error\n";
				} else if (readStr == END) {
					break;
				}
			}
		} catch (boost::system::system_error& err) {
			std::cerr << err.what() << std::endl;
		}
		sock->close();
		// decreaseClientCount();
	}
}

