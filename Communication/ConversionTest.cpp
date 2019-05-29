//#include "ConversionClient.h"
//#include <fstream>
//#include <thread>
//#include <chrono>
//#include <vector>
//#include <tuple>
//
//using namespace std;
//using namespace CanTouchThis;
//
//int main(int argc, char *argv[])
//{
//	ConversionClient client(LOCALHOST, PORT);
//	std::tuple<int, int, double> point;
//
//	std::string curr, obj;
//
//	std::this_thread::sleep_for(chrono::milliseconds(2000));
//	while (client.getLocationData(point)) {
//		std::cout << "Location data has been acquired\n";
//		std::cout << "x: " << std::get<0>(point) << " "
//				  << "y: " << std::get<1>(point) << " "
//				  << "z: " << std::get<2>(point) << std::endl;
//		std::cout << std::endl;
//	}
//
//	return 0;
//}
