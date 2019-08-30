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
	HWND hWnd = ::FindWindow("UnityWndClass", CARDGAME);
	ProcessImage *processImage = ProcessImage::getInstance();
	GameInfo gameInfo;
	AI aiControl(hWnd);
	aiControl.startGame(gameInfo);
	//gameInfo.state = STATE_SELFTURN_FIGHT;
	while (true)
	{
		processImage->process(gameInfo);
		aiControl.process(gameInfo);
		Sleep(1000);
	}

	system("pause");
}