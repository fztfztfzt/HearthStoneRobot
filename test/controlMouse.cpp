#include<Windows.h>
#include<assert.h>
#include<fstream>
#include<iostream>
using namespace std;
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )//避免控制台遮挡
int main()
{
	HWND hWnd = ::FindWindow("UnityWndClass", ("炉石传说"));
	int x = 565;// / 1920 * 65535;
	int y = 635;// / 1200 * 65535;
	//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, 1072.0 / 1920 * 65535, 922.0 / 1200 * 65535, 0, 0);
	//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	SetForegroundWindow(hWnd);
	//::SetActiveWindow(hWnd);
	RECT rc;
	::GetWindowRect(hWnd, &rc);
	::MoveWindow(hWnd, 0, 0, rc.right - rc.left, rc.bottom - rc.top, true);

	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, 520.0 / 1920 * 65535, 253.0 / 1200 * 65535, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	/*::PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
	::PostMessage(hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(x, y));
	::PostMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));*/

	//	system("pause");
}