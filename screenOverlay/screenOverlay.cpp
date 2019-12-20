#include <iostream>
#include "windows.h"
#include "magnification.h"

using namespace std;

float c = (float) 1.1;
float t =  ((float) 1.0 - c) / (float) 2.0;
MAGCOLOREFFECT g_MagEffectTransform = { c,		0.0f,	0.0f,	0.0f,	0.0f,
                                        0.0f,	c,		0.0f,	0.0f,	0.0f,
                                        0.0f,	0.0f,	c,		0.0f,	0.0f,
                                        0.0f,	0.0f,	0.0f,	1.0f,	0.0f,
									    t,		t,		t,		0.0f,	1.0f};

MAGCOLOREFFECT g_MagEffectIdentity = {1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                                      0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
                                      0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                                      0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                                      0.0f,  0.0f,  0.0f,  0.0f,  1.0f};

BOOL transformColor(__in BOOL transformationOn) {// Apply the color matrix 
    PMAGCOLOREFFECT pEffect = (transformationOn ? &g_MagEffectTransform : &g_MagEffectIdentity);
    return MagSetFullscreenColorEffect(pEffect);
}

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
		cout << "Initialized" << endl << endl;
		while (true) {
			if ((GetKeyState(VK_RBUTTON) & 0x100) != 0) {
				SetZoomB(2);
				transformColor(TRUE);
				cout << "Enhance" << endl;
				while ((GetKeyState(VK_RBUTTON) & 0x100) != 0) { Sleep(20); }
			} else {
				SetZoomB(1);
				transformColor(FALSE);
				cout << "Restore" << endl;
				while ((GetKeyState(VK_RBUTTON) & 0x100) == 0) { Sleep(2); }
			}
		}
		Sleep(2);
	}
	return 0;
}
