#include <iostream>
#include "windows.h"
#include "magnification.h"

using namespace std;

MAGCOLOREFFECT g_MagEffectGrayscale = {0.3f,  0.3f,  0.3f,  0.0f,  0.0f,
                                       0.6f,  0.6f,  0.6f,  0.0f,  0.0f,
                                       0.1f,  0.1f,  0.1f,  0.0f,  0.0f,
                                       0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                                       0.0f,  0.0f,  0.0f,  0.0f,  1.0f};

MAGCOLOREFFECT g_MagEffectIdentity = {1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                                      0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
                                      0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                                      0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                                      0.0f,  0.0f,  0.0f,  0.0f,  1.0f};

BOOL SetColorGrayscale(__in BOOL fGrayscaleOn) {
    // Apply the color matrix required to either apply grayscale to the screen 
    // colors or to show the regular colors.
    PMAGCOLOREFFECT pEffect = (fGrayscaleOn ? &g_MagEffectGrayscale : &g_MagEffectIdentity);
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
