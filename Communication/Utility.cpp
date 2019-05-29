#include "Utility.h"

namespace CanTouchThis {
	const std::string LOCALHOST = "127.0.0.1";
	const unsigned short PORT = 9001;

	const std::string SUCCESS = "OK!";
	const std::string FAILURE = "ERR";
	const std::string END = "EOF";

	const std::string CLIENT_SCANNER_LISTEN = "SCANNER_LISTEN";
	const std::string CLIENT_SCANNER = "SCANNER";
	const std::string CLIENT_CONTROLLER = "CONTROLLER";
	const std::string CLIENT_VIEWER = "VIEWER";
	const std::string CLIENT_CONVERSION = "CONVERSION";
	const int MAXIMUM_CLIENTS = 10;
	const int MAXIMUM_BACKLOG = 5;

	/*************************** Operations ***********************************/
	// Scanner Operations
	const std::string OP_SCANNER_SEND_LOCATION = "SCANNER_SEND_LOCATION";
	const std::string OP_SCANNER_LISTEN_CONTROLLER = "SCANNER_LISTEN_CONTROLLER";

   	// Conversion Operations
   	const std::string OP_CONVERSION_SEND_OBJ = "CONVERSION_SEND_OBJ";
   	const std::string OP_CONVERSION_GET_LOC_ONE = "CONVERSION_GET_LOC_ONE";

	// Viewer Operations
	const std::string OP_VIEWER_GET_OBJ = "VIEWER_GET_OBJ";

	// Controller Operations
	const std::string OP_CONTROLLER_COMMAND = "CONTROLLER_COMMAND";

	// Controller Constants
	const std::string CONTROLLER_INIT = "CONTROLLER_INIT";
	const std::string CONTROLLER_START_SCAN = "CONTROLLER_START_SCAN";
	const std::string CONTROLLER_STOP_SCAN = "CONTROLLER_STOP_SCAN";
	const std::string CONTROLLER_SHUTDOWN = "CONTROLLER_SHUTDOWN";

	const std::string CONTROLLER_MOTOR_X_START = "CONTROLLER_MOTOR_X_START";
	const std::string CONTROLLER_MOTOR_X1_START = "CONTROLLER_MOTOR_X1_START";
	const std::string CONTROLLER_MOTOR_X2_START = "CONTROLLER_MOTOR_X2_START";
	const std::string CONTROLLER_MOTOR_Y_START = "CONTROLLER_MOTOR_Y_START";
	const std::string CONTROLLER_MOTOR_Z_START = "CONTROLLER_MOTOR_Z_START";

	const std::string CONTROLLER_MOTOR_Y_STARTR = "CONTROLLER_MOTOR_Y_STARTR";
	const std::string CONTROLLER_MOTOR_Z_STARTR = "CONTROLLER_MOTOR_Z_STARTR";
	const std::string CONTROLLER_MOTOR_X_STARTR = "CONTROLLER_MOTOR_X_STARTR";
	const std::string CONTROLLER_MOTOR_X1_STARTR = "CONTROLLER_MOTOR_X1_STARTR";
	const std::string CONTROLLER_MOTOR_X2_STARTR = "CONTROLLER_MOTOR_X2_STARTR";

	const std::string CONTROLLER_MOTOR_X_STOP = "CONTROLLER_MOTOR_X_STOP";
	const std::string CONTROLLER_MOTOR_X1_STOP = "CONTROLLER_MOTOR_X1_STOP";
	const std::string CONTROLLER_MOTOR_X2_STOP = "CONTROLLER_MOTOR_X2_STOP";
	const std::string CONTROLLER_MOTOR_Y_STOP = "CONTROLLER_MOTOR_Y_STOP";
	const std::string CONTROLLER_MOTOR_Z_STOP = "CONTROLLER_MOTOR_Z_STOP";
	/**************************************************************************/

	std::string readFromSocket(boost::asio::ip::tcp::socket& sock,
							   boost::asio::streambuf& buf)
	{
		std::string str, tmp;
		bool end = false;

		if (buf.size() != 0) {
			end = readUntilNewLine(buf, str);
		} else {
			try {
				boost::asio::read_until(sock, buf, '\n');
				std::getline(std::istream(&buf), str);
			} catch (boost::system::system_error& err) {
				return err.code() == boost::asio::error::eof ? END : FAILURE;
			}
		}
		if (end) {
			try {
				boost::asio::read_until(sock, buf, '\n');
				readUntilNewLine(buf, str);
			} catch (boost::system::system_error& err) {
				return err.code() == boost::asio::error::eof ? END : FAILURE;
			}
		}
		return str;
	}

	bool readUntilNewLine(boost::asio::streambuf& buf, std::string& str)
	{
		std::istream in(&buf);
		char ch;

		while (in.get(ch) && ch != '\n')
			str += ch;
		return ch == '\n' ? false : true;
	}

	std::string lineToJsonString(boost::asio::ip::tcp::socket& sock,
                             boost::asio::streambuf& buf)
	{
		std::string total, current;

		while ((current = readFromSocket(sock, buf)) != "}") {
			if (current == END) return current;
			total += current + "\n";
		}
		total += current;
		return total;
	}

	std::string objToJson(const std::string& objFile)
	{
		boost::property_tree::ptree ptree;
		std::string line;
		int lineCount = 0;
		std::istringstream in(objFile);
		while (getline(in, line)) {
			ptree.put(std::to_string(lineCount), line);
			++lineCount;
		}
		std::ostringstream oss;
		boost::property_tree::json_parser::write_json(oss, ptree);
		return oss.str() + "\n";
	}

	std::string jsonToObj(const std::string& json)
	{
		std::istringstream inputStream(json);
		boost::property_tree::ptree ptree;
		boost::property_tree::read_json(inputStream, ptree);
		std::string theFile;
		for (auto& kv : ptree)
			theFile += ptree.get<std::string>(kv.first) + "\n";
		return theFile + "\n";
	}

	std::string commandToString(Command command)
	{
		std::string value;

		switch (command) {
			case Command::INIT: value = CONTROLLER_INIT; break;
			case Command::START_SCAN: value = CONTROLLER_START_SCAN; break;
			case Command::STOP_SCAN: value = CONTROLLER_STOP_SCAN; break;
			case Command::SHUTDOWN: value = CONTROLLER_SHUTDOWN; break;

			case Command::MOTOR_X_START: value = CONTROLLER_MOTOR_X_START; break;
			case Command::MOTOR_X1_START: value = CONTROLLER_MOTOR_X1_START; break;
			case Command::MOTOR_X2_START: value = CONTROLLER_MOTOR_X2_START; break;
			case Command::MOTOR_Y_START: value = CONTROLLER_MOTOR_Y_START; break;
			case Command::MOTOR_Z_START: value = CONTROLLER_MOTOR_Z_START; break;

			case Command::MOTOR_X_STARTR: value = CONTROLLER_MOTOR_X_STARTR; break;
			case Command::MOTOR_X1_STARTR: value = CONTROLLER_MOTOR_X1_STARTR; break;
			case Command::MOTOR_X2_STARTR: value = CONTROLLER_MOTOR_X2_STARTR; break;
			case Command::MOTOR_Y_STARTR: value = CONTROLLER_MOTOR_Y_STARTR; break;
			case Command::MOTOR_Z_STARTR: value = CONTROLLER_MOTOR_Z_STARTR; break;

			case Command::MOTOR_X_STOP: value = CONTROLLER_MOTOR_X_STOP; break;
			case Command::MOTOR_X1_STOP: value = CONTROLLER_MOTOR_X1_STOP; break;
			case Command::MOTOR_X2_STOP: value = CONTROLLER_MOTOR_X2_STOP; break;
			case Command::MOTOR_Y_STOP: value = CONTROLLER_MOTOR_Y_STOP; break;
			case Command::MOTOR_Z_STOP: value = CONTROLLER_MOTOR_Z_STOP; break;
		}
		return value;
	}

	Command stringToCommand(const std::string& command)
	{
		Command value;

		if (command == CONTROLLER_INIT) value = Command::INIT;
		else if (command == CONTROLLER_START_SCAN) value = Command::START_SCAN;
		else if (command == CONTROLLER_STOP_SCAN) value = Command::STOP_SCAN;
		else if (command == CONTROLLER_SHUTDOWN) value = Command::SHUTDOWN;

		else if (command == CONTROLLER_MOTOR_X_START) value = Command::MOTOR_X_START;
		else if (command == CONTROLLER_MOTOR_X1_START) value = Command::MOTOR_X1_START;
		else if (command == CONTROLLER_MOTOR_X2_START) value = Command::MOTOR_X2_START;
		else if (command == CONTROLLER_MOTOR_Y_START) value = Command::MOTOR_Y_START;
		else if (command == CONTROLLER_MOTOR_Z_START) value = Command::MOTOR_Z_START;

		else if (command == CONTROLLER_MOTOR_X_STARTR) value = Command::MOTOR_X_STARTR;
		else if (command == CONTROLLER_MOTOR_X1_STARTR) value = Command::MOTOR_X1_STARTR;
		else if (command == CONTROLLER_MOTOR_X2_STARTR) value = Command::MOTOR_X2_STARTR;
		else if (command == CONTROLLER_MOTOR_Y_STARTR) value = Command::MOTOR_Y_STARTR;
		else if (command == CONTROLLER_MOTOR_Z_STARTR) value = Command::MOTOR_Z_STARTR;

		else if (command == CONTROLLER_MOTOR_X_STOP) value = Command::MOTOR_X_STOP;
		else if (command == CONTROLLER_MOTOR_X1_STOP) value = Command::MOTOR_X1_STOP;
		else if (command == CONTROLLER_MOTOR_X2_STOP) value = Command::MOTOR_X2_STOP;
		else if (command == CONTROLLER_MOTOR_Y_STOP) value = Command::MOTOR_Y_STOP;
		else if (command == CONTROLLER_MOTOR_Z_STOP) value = Command::MOTOR_Z_STOP;
		return value;
	}
}

