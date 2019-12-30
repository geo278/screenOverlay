#include <iostream>
#include "windows.h"
#include "magnification.h"

using namespace std;
float s = (float) 1.1;
float sr = (float)(1 - s) * (float)0.3086;
float sg = (float)(1 - s) * (float)0.6094;
float sb = (float)(1 - s) * (float)0.0820;
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
	int xSize = GetSystemMetrics(SM_CXSCREEN);
	int ySize = GetSystemMetrics(SM_CYSCREEN);
	if (xSize == 2048 && ySize == 1152) {
		xSize = 2560;
		ySize = 1440;
	}
	int xOffset = (int)((float)xSize * (1.0 - (1.0 / magFactor)) / 2.0);
	int yOffset = (int)((float)ySize * (1.0 - (1.0 / magFactor)) / 2.0);
	//cout << (int)(float)GetSystemMetrics(SM_CXSCREEN) << "  " << (int)(float)GetSystemMetrics(SM_CYSCREEN) << endl;
	return MagSetFullscreenTransform(magFactor, xOffset, yOffset);
}

int main() {
	if (MagInitialize()) {
		cout << "Initialized" << endl << endl;
		while (true) {
			if ((GetKeyState(VK_RBUTTON) & 0x100) != 0) {
				SetZoomB(2);
				//MagSetFullscreenColorEffect(&g_MagEffectSaturation);
				cout << "Zoom In" << endl;
				while ((GetKeyState(VK_RBUTTON) & 0x100) != 0) { Sleep(20); }
			} else {
				SetZoomB(1);
				//MagSetFullscreenColorEffect(&g_MagEffectIdentity);
				cout << "Restore" << endl;
				while ((GetKeyState(VK_RBUTTON) & 0x100) == 0) { Sleep(2); }
			}
		}
		Sleep(2);
	}
	return 0;
}
