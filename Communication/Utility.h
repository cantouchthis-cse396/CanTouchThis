#ifndef __UTILITY_H__
#define __UTILITY_H__
#include <iostream>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace CanTouchThis {
	extern const std::string LOCALHOST;
	extern const unsigned short PORT;

	extern const std::string SUCCESS;
	extern const std::string FAILURE;
	extern const std::string END;

	extern const std::string CLIENT_SCANNER_LISTEN;
	extern const std::string CLIENT_SCANNER;
	extern const std::string CLIENT_CONTROLLER;
	extern const std::string CLIENT_VIEWER;
	extern const std::string CLIENT_CONVERSION;
	extern const int MAXIMUM_CLIENTS;
	extern const int MAXIMUM_BACKLOG;

	/*************************** Operations ***********************************/
	// Scanner Operations
	extern const std::string OP_SCANNER_SEND_LOCATION;
	extern const std::string OP_SCANNER_LISTEN_CONTROLLER;

	// Conversion Operations
	extern const std::string OP_CONVERSION_SEND_OBJ;
   	extern const std::string OP_CONVERSION_GET_LOC_ONE;

	// Viewer Operations
    extern const std::string OP_VIEWER_GET_OBJ;

	// Controller Operations
    extern const std::string OP_CONTROLLER_COMMAND;

    // Controller Constants
	extern const std::string CONTROLLER_INIT;
	extern const std::string CONTROLLER_START_SCAN;
	extern const std::string CONTROLLER_STOP_SCAN;
	extern const std::string CONTROLLER_SHUTDOWN;

	extern const std::string CONTROLLER_MOTOR_X_START;
	extern const std::string CONTROLLER_MOTOR_X1_START;
	extern const std::string CONTROLLER_MOTOR_X2_START;
	extern const std::string CONTROLLER_MOTOR_Y_START;
	extern const std::string CONTROLLER_MOTOR_Z_START;

	extern const std::string CONTROLLER_MOTOR_X_STARTR;
	extern const std::string CONTROLLER_MOTOR_X1_STARTR;
	extern const std::string CONTROLLER_MOTOR_X2_STARTR;
	extern const std::string CONTROLLER_MOTOR_Y_STARTR;
	extern const std::string CONTROLLER_MOTOR_Z_STARTR;

	extern const std::string CONTROLLER_MOTOR_X_STOP;
	extern const std::string CONTROLLER_MOTOR_X1_STOP;
	extern const std::string CONTROLLER_MOTOR_X2_STOP;
	extern const std::string CONTROLLER_MOTOR_Y_STOP;
	extern const std::string CONTROLLER_MOTOR_Z_STOP;
	/**************************************************************************/

	enum class Command { INIT, START_SCAN, STOP_SCAN,
						 MOTOR_X_START, MOTOR_X_STARTR,
						 MOTOR_X1_START, MOTOR_X1_STARTR,
						 MOTOR_X2_STARTR, MOTOR_X2_START,
						 MOTOR_Y_START, MOTOR_Z_START,
						 MOTOR_Y_STARTR, MOTOR_Z_STARTR,
						 MOTOR_X_STOP, MOTOR_X1_STOP, MOTOR_X2_STOP,
						 MOTOR_Y_STOP, MOTOR_Z_STOP, SHUTDOWN};

	extern std::string readFromSocket(boost::asio::ip::tcp::socket& sock,
							   	   	  boost::asio::streambuf& buf);
	extern std::string lineToJsonString(boost::asio::ip::tcp::socket& sock,
							   	   	  boost::asio::streambuf& buf);
	extern bool readUntilNewLine(boost::asio::streambuf& buf, std::string& again);
	extern std::string objToJson(const std::string& objFile);
	extern std::string jsonToObj(const std::string& json);
	extern std::string commandToString(Command command);
	extern Command stringToCommand(const std::string& command);
}
#endif

