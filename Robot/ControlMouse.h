#pragma once

#include<Windows.h>

class ControlMouse
{
public:
	static ControlMouse *getInstance();
	void touchPosition(double x, double y);//�������λ��
	void playCard(double x, double y);//����
	void playCardToPlayer(double x, double y);
	void moveToPosition(double x, double y);//�ƶ�������λ��
	void fightMonster(double x, double y, double x1, double y1);
private:
	HWND hWnd;
	int screenWidth;
	int screenHeight;
	static ControlMouse* self;
	ControlMouse();
};
