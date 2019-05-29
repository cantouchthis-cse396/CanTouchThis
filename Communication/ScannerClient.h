#ifndef __SCANNER_CLIENT_H__
#define __SCANNER_CLIENT_H__
#include "Client.h"
#include <condition_variable>
#include <mutex>

namespace CanTouchThis {

	class ScannerClient : public Client {
		public:
			/*
			ScannerClient(const std::string& ip, unsigned short port,
						  Command& comm, std::condition_variable& cond);*/
			ScannerClient(const std::string& ip, unsigned short port, Command& comm);
			~ScannerClient();

			bool sendLocationInfo(int x, int y, double z);
			void listenToController();
		private:

			struct Listener : public Client {
				Listener(const std::string& ip, unsigned short port);
				~Listener();
				void listen(Command& command, std::condition_variable& cond, bool& isClicked);
				std::shared_ptr<std::thread> th;
			};
			Command& command;
			Listener listen;
			std::mutex m;
			std::condition_variable cond;
			bool isClicked;
	};
}
#endif

