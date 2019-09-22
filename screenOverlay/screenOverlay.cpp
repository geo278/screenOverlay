// screenOverlay.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

HDC dc = GetDC(HWND_DESKTOP);
COLORREF colour = RGB(255, 0, 0); // red
int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);
int width = 160;
int height = 160;

void row(int width) {
	for (int i = 0; i < width; i++) {
		SetPixel(dc, i + screenWidth / 2 - width / 2, screenHeight / 2 - 1, colour);
	}
	for (int i = 0; i < width; i++) {
		SetPixel(dc, i + screenWidth / 2 - width / 2, screenHeight / 2, colour);
	}
}
void col(int height) {
	for (int i = 0; i < height; i++) {
		SetPixel(dc, screenWidth / 2 - 1, i + screenHeight / 2 - height / 2, colour);
	}
	for (int i = 0; i < height; i++) {
		SetPixel(dc, screenWidth / 2, i + screenHeight / 2 - height / 2, colour);
	}
}

RGBQUAD* capture(POINT a, POINT b) {
	// copy screen to bitmap
	HDC     hScreen = GetDC(NULL);
	HDC     hDC = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, abs(b.x - a.x), abs(b.y - a.y));
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
	BOOL    bRet = BitBlt(hDC, 0, 0, abs(b.x - a.x), abs(b.y - a.y), hScreen, a.x, a.y, SRCCOPY); // BitBlt does the copying

	/*
	// save bitmap to clipboard
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, hBitmap);
	CloseClipboard();
	*/

	// Array conversion:
	RGBQUAD* pixels = new RGBQUAD[width * height];

	BITMAPINFOHEADER bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biPlanes = 1;
	bmi.biBitCount = 32;
	bmi.biWidth = width;
	bmi.biHeight = -height;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0;// 3 * ScreenX * ScreenY;

	GetDIBits(hDC, hBitmap, 0, height, pixels, (BITMAPINFO*)& bmi, DIB_RGB_COLORS);

	int displacement = 200;
	// renders pixels on screen at displacement lower than center
	SetDIBitsToDevice(dc, a.x, a.y + displacement, width, height, 0, 0, 0, height, pixels, (BITMAPINFO*)& bmi, DIB_RGB_COLORS);


	// clean up
	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);
	return pixels;
}

int main() {
	POINT a, b;
	a.x = 1920 / 2 - width / 2;
	a.y = 1080 / 2 - height / 2;
	b.x = 1920 / 2 + width / 2;
	b.y = 1080 / 2 + height / 2;
	RGBQUAD* pixels;
	while (true) {

		pixels = capture(a, b);





		Sleep(10);
	}
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE) CrossThread, 0, 0, 0);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
