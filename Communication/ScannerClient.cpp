#include "ScannerClient.h"
#include <thread>

namespace CanTouchThis {

	ScannerClient::ScannerClient(const std::string& ip, unsigned short port,
								 Command& comm)
		 : Client(ip, port, CLIENT_SCANNER), command(comm), listen(ip, port),
		   isClicked(false)
	{
		listen.listen(command, cond, isClicked);
	}

	ScannerClient::~ScannerClient()
	{ }

	bool ScannerClient::sendLocationInfo(int x, int y, double z)
	{
		try {
			std::string op = OP_SCANNER_SEND_LOCATION + "\n";
			boost::asio::write(*socketptr, boost::asio::buffer(op));

			std::ostringstream oss;
			boost::property_tree::ptree ptree;
			char response[3] = "";

			ptree.put("x", x);
			ptree.put("y", y);
			ptree.put("z", z);

			boost::property_tree::json_parser::write_json(oss, ptree);
			boost::asio::write(*socketptr, boost::asio::buffer(oss.str() + "\n"));
			boost::asio::read(*socketptr, boost::asio::buffer(response, 3));
			if (strcmp(response, FAILURE.c_str()) == 0)
				return false;
		} catch (boost::system::system_error& err) {
			std::cout << err.what() << std::endl;
			return false;
		}
		return true;
	}

	void ScannerClient::listenToController()
	{
		{
			std::unique_lock<std::mutex> lock(m);
			cond.wait(lock, [this]{
				return this->isClicked;
			});
			isClicked = false;
		}
	}

	ScannerClient::Listener::Listener(const std::string& ip, unsigned short port)
				 : Client(ip, port, CLIENT_SCANNER_LISTEN)
	{ }

	ScannerClient::Listener::~Listener()
	{
		th->join();
	}

	void ScannerClient::Listener::listen(Command& command,
					std::condition_variable& cond, bool& isClicked)
	{
		th.reset(new std::thread(([this, &command, &cond, &isClicked]() {
			command = Command::INIT;
			try {
				std::string op = OP_SCANNER_LISTEN_CONTROLLER + "\n";
				std::string readStr = "";
				while (command != Command::SHUTDOWN) {
					boost::asio::write(*socketptr, boost::asio::buffer(op));
					boost::asio::streambuf buf;
					if ((readStr = lineToJsonString(*socketptr, buf)) == END)
						break;
					std::istringstream iss(readStr);
					boost::property_tree::ptree ptree;
					boost::property_tree::json_parser::read_json(iss, ptree);
					command = stringToCommand(ptree.get<std::string>("command"));
					isClicked = true;
					cond.notify_one();
				}
			} catch (boost::system::system_error& err) {
				std::cout << err.what() << std::endl;
			}
		})));
	}
}

