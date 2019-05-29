#ifndef __COMM_SERVER_H__
#define __COMM_SERVER_H__
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <cstdlib>
#include <condition_variable>
#include <mutex>
#include <map>
#include <memory>
#include <vector>
#include <tuple>
#include <atomic>
#include "Utility.h"

namespace CanTouchThis {
	class CommServer {
		public:
			CommServer(unsigned short port, bool start=false);
			~CommServer();
			void startTheServer(void);

		private:
			void acceptClients(void);
			std::string streamBufferToString(const boost::asio::streambuf& buf) const;
			void scannerServices(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
								 std::shared_ptr<boost::asio::streambuf> buf);
			void scannerListener(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
								 std::shared_ptr<boost::asio::streambuf> buf);
			void controllerServices(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
								 std::shared_ptr<boost::asio::streambuf> buf);
			void conversionServices(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
								 std::shared_ptr<boost::asio::streambuf> buf);
			void viewerServices(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
								 std::shared_ptr<boost::asio::streambuf> buf);
			void handleClient(const std::string& clientType,
					std::shared_ptr<boost::asio::ip::tcp::socket> sock,
					std::shared_ptr<boost::asio::streambuf> buf);
			void decreaseClientCount(void);

		private:
			std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;
			std::vector<std::thread> threads;
			boost::asio::io_service ioContext;

			std::vector <std::tuple<int, int, double> > locationData;
			std::condition_variable locationCond;
			std::mutex locationMutex;
			long unsigned int lastLocation;

			std::vector <std::string> objFiles;
			std::condition_variable objCond;
			std::mutex objMutex;

			std::condition_variable finishCond;
			std::mutex finishMutex;

			Command control;
			bool checkLast;
			std::condition_variable controlCond;
			std::mutex controlMutex;

			int clientCount;
			bool newObj;
	};
}
#endif

