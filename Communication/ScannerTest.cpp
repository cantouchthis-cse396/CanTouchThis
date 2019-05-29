//#include "ScannerClient.h"
//#include <fstream>
//#include <thread>
//#include <chrono>
//#include <mutex>
//#include <condition_variable>
//
//using namespace std;
//using namespace CanTouchThis;
//
//int main(int argc, char *argv[])
//{
//	Command command;
//	ScannerClient client(LOCALHOST, PORT, command);
//	int x, y;
//	double z;
//	std::ifstream inFile("test_aq");
//
//	while (inFile >> x >> y >> z) {
//		client.sendLocationInfo(x, y, z);
//		std::cout << "x: " << x << " - y: " << y << " - z: " << z << std::endl;
//		std::this_thread::sleep_for(std::chrono::milliseconds(50));
//	}
//	return 0;
//}
//
