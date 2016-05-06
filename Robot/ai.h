#pragma once
#include <Windows.h>
#include "gameInfo.h"
#include "ControlMouse.h"
#include "define.h"
class AI
{
	HWND hWnd;
	ControlMouse *controlMouse;
public:
	AI(HWND h) :hWnd(h){
		controlMouse = new ControlMouse(hWnd);
	}
	void process(GameInfo &gameInfo)
	{
		switch (gameInfo.state)
		{
		case STATE_STARTGAME:
			cout << "游戏阶段：开始游戏" << endl;
			controlMouse->touchPosition(800, 600);//点击开始
			Sleep(1000);
			gameInfo.state = STATE_CHANGECARDSTART;
			break;
		case STATE_CHANGECARDSTART:
			cout << "游戏阶段：换牌阶段开始" << endl;
			if (gameInfo.currentNum == 3) gameInfo.first = true;
			else gameInfo.first = false;
			for (int i = 0; i < gameInfo.currentNum; ++i)
			{
				if (gameInfo.handCard[i].spend >= 2)
				{
					controlMouse->touchPosition(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
					Sleep(100);
				}
			}
			gameInfo.state = STATE_CHANGECARDEND;
			break;
		case STATE_CHANGECARDEND:
			cout << "游戏阶段：换牌阶段结束" << endl;
			//确定
			controlMouse->touchPosition(500, 600);
			Sleep(100);
			gameInfo.state = STATE_FIGHTSTART;
			break;
		case STATE_FIGHTSTART:
			cout << "游戏阶段：开始战斗阶段" << endl;
			Sleep(5000);//等待发牌结束
			gameInfo.state = STATE_OTHERTURN;
			break;
		case STATE_SELFTURN:
			cout << "游戏阶段：自己出牌阶段" << endl;
			gameInfo.currentSpend++;
			gameInfo.couldUseSpend = gameInfo.currentSpend;
			for (int i = 0; i < gameInfo.currentNum; ++i)
			{
				if (gameInfo.handCard[i].spend <= gameInfo.couldUseSpend)
				{
					cout << "ai:出牌：费用" << gameInfo.handCard[i].spend << "位置：" << gameInfo.handCard[i].x << " " << gameInfo.handCard[i].y << endl;
					controlMouse->playCard(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
					gameInfo.couldUseSpend -= gameInfo.handCard[i].spend;
					Sleep(100);
				}
			}

			for (int i = 0; i < 10; ++i)
			{
				controlMouse->playCardToPlayer(140+60*i,460);
			}
			controlMouse->touchPosition(900, 350);//点击回合结束
			gameInfo.state = STATE_OTHERTURN;
			break;
		case STATE_OTHERTURN:
			cout << "游戏阶段：对手出牌阶段" << endl;
			break;
		default:
			break;
		}
		//controlMouse->moveToPosition(OutsideX, OutsideY);
	}
};