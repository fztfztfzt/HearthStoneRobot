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
int main()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rc = { 1400, 0, 120, 30 };
	SetConsoleWindowInfo(hOut, true, &rc);
	HWND hWnd = ::FindWindow("UnityWndClass", ("炉石传说"));
	ProcessImage *processImage =ProcessImage::getInstance();
	GameInfo gameInfo;
	AI aiControl(hWnd);
	while (1)
	{
		processImage->process(gameInfo);
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