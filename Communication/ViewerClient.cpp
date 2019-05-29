#include "ViewerClient.h"

namespace CanTouchThis {

	ViewerClient::ViewerClient(const std::string& ip, unsigned short port)
			   : Client(ip, port, CLIENT_VIEWER)
	{ /* Intentionally left blank */ }

	ViewerClient::~ViewerClient()
	{ /* May be used in future */ }

	bool ViewerClient::getObjFile(std::string& fileContent)
	{
		try {
			std::string op = OP_VIEWER_GET_OBJ + "\n";
			boost::asio::write(*socketptr, boost::asio::buffer(op));
			boost::asio::streambuf buf;
			fileContent = jsonToObj(lineToJsonString(*socketptr, buf));
		} catch (boost::system::system_error& err) {
			std::cout << err.what() << std::endl;
			return false;
		}
		return true;
	}
}

