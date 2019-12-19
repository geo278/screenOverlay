#include <iostream>
#include "windows.h"
#include "magnification.h"

using namespace std;

BOOL SetZoomB(float magFactor) {
	if (magFactor < 1.0) {
		return FALSE;
	}
	int xOffset = (int)((float)GetSystemMetrics(SM_CXSCREEN) * (1.0 - (1.0 / magFactor)) / 2.0);
	int yOffset = (int)((float)GetSystemMetrics(SM_CYSCREEN) * (1.0 - (1.0 / magFactor)) / 2.0);
	return MagSetFullscreenTransform(magFactor, xOffset, yOffset);
}

int main() {
	if (MagInitialize()) {
		cout << "Initialized" << endl;
		while (true) {
			if ((GetKeyState(VK_RBUTTON) & 0x100) != 0) {
				SetZoomB(2);
				cout << "Zoom In" << endl;
				while ((GetKeyState(VK_RBUTTON) & 0x100) != 0) { Sleep(20); }
			} else {
				SetZoomB(1);
				cout << "Zoom Out" << endl;
				while ((GetKeyState(VK_RBUTTON) & 0x100) == 0) { Sleep(2); }
			}
		}
		Sleep(2);
	}
	return 0;
}
