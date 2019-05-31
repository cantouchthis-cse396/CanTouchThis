#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <boost/asio.hpp>

#ifdef _WIN64
  #include <SDKDDKVer.h>
#endif

#include <iostream>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <cstring>
#include <atomic>
#include <exception>
#include "Utility.h"

namespace CanTouchThis {

	class Client {
		public:
			~Client();
			void close(void);

		protected:
			Client();
			Client(const std::string& ip, unsigned short port,
				   const std::string& type);
			void start(const std::string& type);

			std::shared_ptr<boost::asio::ip::tcp::socket> socketptr;
			boost::asio::ip::tcp::endpoint ep;
			boost::asio::io_service ioContext;
	};
}

#endif

