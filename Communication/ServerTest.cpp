#include "CommServer.h"

int main(int argc, char *argv[]) {
#ifdef _WIN64
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
#endif

	CanTouchThis::CommServer server(CanTouchThis::PORT, true);

	return 0;
}
