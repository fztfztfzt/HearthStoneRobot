#include<Windows.h>
#include<assert.h>
#include<fstream>
#include<iostream>
#include <opencv2\opencv.hpp>
#include "processImage.h"
#include "gameInfo.h"
#include "ControlMouse.h"
#include "ai.h"
using namespace std;
using namespace cv;
//#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
Mat getCurrentImage(HWND hWnd)
{
	HDC hDC = ::GetWindowDC(hWnd);//得到句柄
	assert(hDC);

	HDC hMemDC = ::CreateCompatibleDC(hDC);//建立兼容dc

	assert(hMemDC);

	RECT rc;
//	::GetWindowRect(hWnd, &rc);//此矩形包括程序外边框
	::GetClientRect(hWnd, &rc);//得到界面部分矩形大小
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);//兼容位图
	assert(hBitmap);

	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, hBitmap);

	::PrintWindow(hWnd, hMemDC, PW_CLIENTONLY);//得到画面

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	BITMAPINFOHEADER bi = { 0 };
	CONST int nBitCount = 24;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = height;
	bi.biPlanes = 1;
	bi.biBitCount = nBitCount;
	bi.biCompression = BI_RGB;
	DWORD dwSize = width * 3 * height;
	unsigned char *data = new unsigned char[dwSize];
	::GetDIBits(hMemDC, hBitmap, 0, height, data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);//将画面输出到data中
	Mat image(height, width, CV_8UC3, data);//将uchar转换为mat类型，用于opencv
	flip(image, image, 0);//图像上下反转

	::SelectObject(hMemDC, hOldBmp);
	::DeleteObject(hBitmap);
	::DeleteObject(hMemDC);
	::ReleaseDC(hWnd, hDC);
	return image;
}
int main()
{
	HWND hWnd = ::FindWindow("UnityWndClass", ("炉石传说"));
	ProcessImage processImage;
	GameInfo gameInfo;
	AI aiControl(hWnd);
	while (1)
	{
		Mat image = getCurrentImage(hWnd);
		processImage.process(image, gameInfo);
		aiControl.process(gameInfo);
		Sleep(1000);
	}
	
	//ControlMouse control(hWnd);
	//control.playCard(450, 745);
//	Mat image = getCurrentImage(hWnd);
////	imshow("test", image);
//	processImage process;
//	GameInfo cinfo;
//	process.process(image,cinfo);
//	ControlMouse controlMouse(hWnd);
//	for (int i = 0; i < cinfo.currentNum; ++i)
//		cout << "cardnum:" << cinfo.handCard[i].spend << endl;
//	for (int i = 0; i < cinfo.currentNum; ++i)
//	{
//		if (cinfo.handCard[i].spend >= 2)
//		{
//			cout << cinfo.handCard[i].x << " " << cinfo.handCard[i].y << endl;
//			controlMouse.touchPosition(cinfo.handCard[i].x, cinfo.handCard[i].y);
//			Sleep(100);
//		}
//	}
//	//确定
//	controlMouse.touchPosition(500, 600);
//	Sleep(100);
	system("pause");
}