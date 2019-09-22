// screenOverlay.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

HDC dc = GetDC(HWND_DESKTOP);
int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);
const int width = 180;
const int height = 180;

/*
COLORREF colour = RGB(255, 0, 0); // red
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
*/

RGBQUAD* zoom(RGBQUAD* pixels) {
	const int size = width * height * 4;
	static RGBQUAD result[size];
	int resultX, resultY, pixelsX, pixelsY;
	for (int i = 0; i < size; i++) {
		resultX = i % (width * 2);
		resultY = i / (width * 2);
		pixelsX = resultX / 2;
		pixelsY = resultY / 2;
		result[i] = pixels[pixelsY * width + pixelsX];
	}
	return result;
}

void capture(POINT a, POINT b) {
	// copy screen to bitmap
	HDC     hScreen = GetDC(NULL);
	HDC     hDC = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, abs(b.x - a.x), abs(b.y - a.y));
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
	BOOL    bRet = BitBlt(hDC, 0, 0, abs(b.x - a.x), abs(b.y - a.y), hScreen, a.x, a.y, SRCCOPY); // BitBlt does the copying

	// Array conversion:
	RGBQUAD* pixels = new RGBQUAD[width * height];

	BITMAPINFOHEADER bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biPlanes = 1;
	bmi.biBitCount = 32;
	bmi.biWidth = width;
	bmi.biHeight = -height;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0; // 3 * ScreenX * ScreenY

	GetDIBits(hDC, hBitmap, 0, height, pixels, (BITMAPINFO*)& bmi, DIB_RGB_COLORS);

	BITMAPINFOHEADER bmiZoom = { 0 };
	bmiZoom.biSize = sizeof(BITMAPINFOHEADER);
	bmiZoom.biPlanes = 1;
	bmiZoom.biBitCount = 32;
	bmiZoom.biWidth = width * 2;
	bmiZoom.biHeight = -height * 2;
	bmiZoom.biCompression = BI_RGB;
	bmiZoom.biSizeImage = 0; // 3 * ScreenX * ScreenY

	int displacement = 200;
	// renders pixels on screen at displacement lower than center
	SetDIBitsToDevice(dc, a.x, a.y + displacement, width * 2, height * 2, 0, 0, 0, height * 2, zoom(pixels), (BITMAPINFO*)& bmiZoom, DIB_RGB_COLORS); // will need to update parameters after implementation of zoom

	// clean up
	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);
	delete[] pixels;
}

int main() {
	POINT a, b;
	a.x = screenWidth / 2 - width / 2;
	a.y = screenHeight / 2 - height / 2;
	b.x = screenWidth / 2 + width / 2;
	b.y = screenHeight / 2 + height / 2;
	while (true) {
		capture(a, b);
		Sleep(1);
	}
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
