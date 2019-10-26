#pragma once

#include<Windows.h>

class ControlMouse
{
public:
	static ControlMouse *getInstance();
	void touchPosition(double x, double y);//点击坐标位置
	void playCard(double x, double y);//出牌
	void playCardToPlayer(double x, double y);
	void moveToPosition(double x, double y);//移动到坐标位置
	void fightMonster(double x, double y, double x1, double y1);
private:
	HWND hWnd;
	int screenWidth;
	int screenHeight;
	static ControlMouse* self;
	ControlMouse();
};
