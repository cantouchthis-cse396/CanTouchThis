#include "ControllerClient.h"

namespace CanTouchThis {

	ControllerClient::ControllerClient(const std::string& ip, unsigned short port)
					: Client(ip, port, CLIENT_CONTROLLER)
	{ /* Intentionally left blank */ }

	ControllerClient::~ControllerClient()
	{ /* May be used in future */ }

	bool ControllerClient::commandScanner(Command command)
	{
		try {
			std::string op = OP_CONTROLLER_COMMAND + "\n";
			boost::asio::write(*socketptr, boost::asio::buffer(op));
			boost::property_tree::ptree ptree;
			std::string value = commandToString(command);
			char response[3] = "";
			ptree.put("command", value);
			std::ostringstream oss;
			boost::property_tree::json_parser::write_json(oss, ptree);
			boost::asio::write(*socketptr, boost::asio::buffer(oss.str() + "\n"));
			boost::asio::read(*socketptr, boost::asio::buffer(response, 3));
			if (strcmp(response, FAILURE.c_str()) == 0) return false;
		} catch (boost::system::system_error& err) {
			std::cout << err.what() << std::endl;
			return false;
		}
		return true;
	}

}

