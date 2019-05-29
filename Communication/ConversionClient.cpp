#include "ConversionClient.h"

namespace CanTouchThis {

	ConversionClient::ConversionClient(const std::string& ip, unsigned short port)
					: Client(ip, port, CLIENT_CONVERSION)
	{ /* Intentionally left blank */ }

	ConversionClient::~ConversionClient()
	{ /* May be used in future */ }

	bool ConversionClient::getLocationData(std::tuple<int, int, double>& data)
	{
		try {
			std::string op = OP_CONVERSION_GET_LOC_ONE + "\n";
			boost::asio::write(*socketptr, boost::asio::buffer(op));

			boost::asio::streambuf buf;
			std::string readStr = lineToJsonString(*socketptr, buf);

			int x, y;
			double z;
			std::string::size_type sz, total = 0;
			std::string l;
			std::istringstream iss(readStr);
			boost::property_tree::ptree ptree;
			boost::property_tree::json_parser::read_json(iss, ptree);
			l = ptree.get<std::string>("location");
			x = std::stoi(l, &sz);
			total += sz;
			y = std::stoi(l.substr(sz), &sz);
			total += sz;
			z = std::stod(l.substr(total));
			data = std::make_tuple(x, y, z);
		} catch (boost::system::system_error& err) {
			std::cout << err.what() << std::endl;
			return false;
		}
		return true;
	}

	bool ConversionClient::sendObjFile(const std::string& fileContent)
	{
		try {
			std::string op = OP_CONVERSION_SEND_OBJ + "\n";
			boost::asio::write(*socketptr, boost::asio::buffer(op));
			char response[3] = "";

			std::string objFile = objToJson(fileContent);
			boost::asio::write(*socketptr, boost::asio::buffer(objFile));
			boost::asio::read(*socketptr, boost::asio::buffer(response, 3));
			if (strcmp(response, FAILURE.c_str()) == 0)
				return false;
		} catch (boost::system::system_error& err) {
			std::cout << err.what() << std::endl;
			return false;
		}
		return true;
	}

}

