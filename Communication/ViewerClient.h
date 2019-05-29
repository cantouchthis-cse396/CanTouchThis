#ifndef __VIEWER_CLIENT_H__
#define __VIEWER_CLIENT_H__
#include "Client.h"

namespace CanTouchThis {
		
	class ViewerClient : public Client {
		public:
			ViewerClient(const std::string& ip, unsigned short port);
			~ViewerClient();

			bool getObjFile(std::string& fileContent);
	};
}
#endif

