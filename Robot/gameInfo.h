#pragma once
#include <iostream>
#include <string>
#include "define.h"
class Card
{
public:
	std::string name;
	std::string explain;
	std::string behavior;
	int spend;
	int attack;
	int life;
	std::string type;
	int x, y;//位置
	bool taugh;//是否嘲讽
	Card(){}
	/*Card(const Card& card)
	{
		name = card.name;
		explain = card.explain;
		behavior = card.behavior;
		spend = card.spend;
		attack = card.attack;
		life = card.life;
		type = card.type;
	}*/
};
class GameInfo
{
public:
	GameInfo()
	{
		state = STATE_STARTGAME;
		currentSpend = 1;
		couldUseSpend = 1;
		haveWeapon = false;
		currentTimes = 0;
	}
	int currentTimes;
	STATE state;
	Card handCard[10];//手牌大小
	Card selfMonster[10];//自己场上的随从
	Card otherMonster[10];//对方场上随从
	int currentNum;//手牌数目
	int selfMonsterNum;
	int otherMonsterNum;
	bool first;//先后手
	int currentSpend;//当前水晶数
	int couldUseSpend;
	bool haveWeapon;

};
