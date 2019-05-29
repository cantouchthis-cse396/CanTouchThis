#include "CommServer.h"

int main(int argc, char *argv[]) {
#ifdef _WIN64
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
#endif

	CanTouchThis::CommServer server(9001, true);

	return 0;
}
