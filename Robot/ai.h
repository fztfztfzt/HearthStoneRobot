#pragma once
#include "define.h"
#include "gameInfo.h"
#include "ControlMouse.h"
#include "processImage.h"
#include <algorithm>


class AI
{
	ControlMouse *controlMouse;
	int havePlayNum;
public:
	AI();
	void startGame(GameInfo &gameInfo);
	void changeCardStart(GameInfo &gameInfo);
	
	void process(GameInfo &gameInfo);
};