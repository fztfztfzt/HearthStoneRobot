#pragma once
#include <iostream>
#include "define.h"
class Card
{
public:
	int spend;
	int attack;
	int life;
	int type;
	int x, y;
};
class GameInfo
{
public:
	GameInfo()
	{
		state = STATE_STARTGAME;
		currentSpend = 0;
	}
	STATE state;
	Card handCard[10];//手牌大小
	int currentNum;//手牌数目
	bool first;//先后手
	int currentSpend;//当前水晶数
	int couldUseSpend;

};
