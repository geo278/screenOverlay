#include <iostream>
#include "windows.h"
#include "magnification.h"

using namespace std;

bool killable = true;
float s = (float) 1.1;
float sr = (float)(1 - s) * (float)0.3086;
float sg = (float)(1 - s) * (float)0.6094;
float sb = (float)(1 - s) * (float)0.0820;
int xSize = GetSystemMetrics(SM_CXSCREEN);
int ySize = GetSystemMetrics(SM_CYSCREEN);
float zoom = 2;
MAGCOLOREFFECT g_MagEffectSaturation = {sr + s,	sr,		sr,		0.0f,	0.0f,
										sg,		sg + s,	sg,		0.0f,	0.0f,
										sb,		sb,		sb + s,	0.0f,	0.0f,
                                        0.0f,	0.0f,	0.0f,	1.0f,	0.0f,
										0.0f,	0.0f,	0.0f,	0.0f,	1.0f};

MAGCOLOREFFECT g_MagEffectIdentity = {1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                                      0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
                                      0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                                      0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                                      0.0f,  0.0f,  0.0f,  0.0f,  1.0f};

BOOL SetZoomB(float magFactor) {
	if (magFactor < 1.0) {
		return FALSE;
	}
	xSize = GetSystemMetrics(SM_CXSCREEN);
	ySize = GetSystemMetrics(SM_CYSCREEN);
	//cout << xSize << "  " << ySize << endl;
	if (xSize == 2048 && ySize == 1152) {
		xSize = 2560;
		ySize = 1440;
	}
	if (xSize == 2133 && ySize == 1200) {
		xSize = 3200;
		ySize = 1800;
	}
	int xOffset = (int)((float)xSize * (1.0 - (1.0 / magFactor)) / 2.0);
	int yOffset = (int)((float)ySize * (1.0 - (1.0 / magFactor)) / 2.0);
	//cout << (int)(float)GetSystemMetrics(SM_CXSCREEN) << "  " << (int)(float)GetSystemMetrics(SM_CYSCREEN) << endl;
	return MagSetFullscreenTransform(magFactor, xOffset, yOffset);
}

void reticule() {
	HDC dc = GetDC(HWND_DESKTOP);
	BITMAPINFOHEADER bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biPlanes = 1;
	bmi.biBitCount = 32;
	bmi.biWidth = 2;
	bmi.biHeight = -2;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0; // 3 * ScreenX * ScreenY
	RGBQUAD centerColour;
	centerColour.rgbRed = 0;
	centerColour.rgbBlue = 0;
	centerColour.rgbGreen = 255;
	RGBQUAD pixels[4] = {centerColour, centerColour, centerColour, centerColour};
	RGBQUAD* p;
	p = pixels;
	while (true) {
		SetDIBitsToDevice(dc, xSize / 2 - 1, ySize / 2 - 1, 2, 2, 0, 0, 0, 2, p, (BITMAPINFO*)&bmi, DIB_RGB_COLORS); // will need to update parameters after implementation of zoom
		Sleep(3);
	}
}

void trackKillable() {
	while (true) {
		if ((GetKeyState(VK_F1) & 0x100) != 0) {
			killable = !killable;
			killable ? cout << "Killable" << endl : cout << "Not Killable" << endl;
			while ((GetKeyState(VK_F1) & 0x100) != 0) {
				Sleep(40);
			}
		}
		Sleep(10);
	}
}

//void trackZoomInput() {
//	while (true) {
//		cout << "enter zoom factor (current: " << zoom << ")" << endl;
//		cin >> zoom;
//		Sleep(50);
//	}
//}

void trackZoom() {
	if (MagInitialize()) {
		cout << "Initialized" << endl << endl;
		while (true) {
			if ((GetKeyState(VK_RBUTTON) & 0x100) != 0) {
				SetZoomB(zoom);
				//MagSetFullscreenColorEffect(&g_MagEffectSaturation);
				cout << "Zoom In" << endl;
				while ((GetKeyState(VK_RBUTTON) & 0x100) != 0) { Sleep(10); }
			} else {
				SetZoomB(1);
				//MagSetFullscreenColorEffect(&g_MagEffectIdentity);
				cout << "Restore" << endl;
				while ((GetKeyState(VK_RBUTTON) & 0x100) == 0) { Sleep(10); }
			}
			Sleep(5);
		}
	}
}

int main() {
	Sleep(5000);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)reticule, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)trackZoom, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)trackKillable, 0, 0, 0);

	while (true) {
		if ((GetKeyState(VK_MENU) & 0x100) != 0 && (GetKeyState(VK_TAB) & 0x100) != 0 && killable) {
			// self revive
			LPCTSTR open = L"open";
			LPCWSTR path = L"C:\\Users\\georg\\Documents\\GitHub\\screenOverlay\\x64\\Debug\\screenOverlay.exe";
			ShellExecute(NULL, open, path, NULL, NULL, SW_SHOWDEFAULT);

			// kill
			break;
		}
		Sleep(20);
	}
	return 0;
}
