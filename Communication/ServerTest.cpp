#include "CommServer.h"

int main(int argc, char *argv[]) {
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	CanTouchThis::CommServer server(9001, true);

	return 0;
}
