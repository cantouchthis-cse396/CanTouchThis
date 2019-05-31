#include "Client.h"

namespace CanTouchThis {
	Client::Client(const std::string& ip, unsigned short port,
				   const std::string& type)
		: ep(boost::asio::ip::address::from_string(ip), port)
	{
		start(type);
	}

	Client::~Client()
	{
		if (socketptr->is_open())
			socketptr->close();
	}

	void Client::start(const std::string& type)
	{
		socketptr.reset(new boost::asio::ip::tcp::socket(ioContext,
						ep.protocol()));
		try {
			socketptr->connect(ep);
			boost::asio::write(*socketptr, boost::asio::buffer(type + "\n"));
		} catch (boost::system::system_error& err) {
			std::cerr << err.what() << std::endl;
			throw std::exception();
		}
	}

	void Client::close(void)
	{
		try {
			boost::asio::write(*socketptr, boost::asio::buffer(END, 3));
			socketptr->shutdown(boost::asio::socket_base::shutdown_send);
			socketptr->close();
		} catch (boost::system::system_error& err) {

		}
	}
}

