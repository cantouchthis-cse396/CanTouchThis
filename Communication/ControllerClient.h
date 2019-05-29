#ifndef __CONTROLLER_CLIENT_H__
#define __CONTROLLER_CLIENT_H__
#include "Client.h"

namespace CanTouchThis {

	class ControllerClient : public Client {
		public:
			ControllerClient(const std::string& ip, unsigned short port);
			~ControllerClient();

			bool commandScanner(Command command);
	};
}
#endif

