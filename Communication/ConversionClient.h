#ifndef __CONVERSION_CLIENT_H__
#define __CONVERSION_CLIENT_H__
#include "Client.h"

namespace CanTouchThis {

	class ConversionClient : public Client {
		public:
			ConversionClient(const std::string& ip, unsigned short port);
			~ConversionClient();

			bool getLocationData(std::tuple<int, int, double>& data);
			bool sendObjFile(const std::string& fileContent);
	};
}

#endif

