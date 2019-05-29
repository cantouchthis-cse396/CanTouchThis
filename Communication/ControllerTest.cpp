//#include "ControllerClient.h"
//#include <fstream>
//
//using namespace std;
//using namespace CanTouchThis;
//
//int main(int argc, char *argv[])
//{
//	ControllerClient client(LOCALHOST, PORT);
//	Command command = Command::INIT;
//	std::string in = "";
//
//	while (true) {
//		std::cin >> in;
//		if (in == "shutdown") {
//			command = Command::SHUTDOWN;
//			client.commandScanner(command);
//			break;
//		} else if (in == "start") {
//			command = Command::START_SCAN;
//			client.commandScanner(command);
//		} else if (in == "stop") {
//			command = Command::STOP_SCAN;
//			client.commandScanner(command);
//		} 
//		command = Command::INIT;
//	}
//	return 0;
//}
//
