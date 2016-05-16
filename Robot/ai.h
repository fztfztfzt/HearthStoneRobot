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
		controlMouse = ControlMouse::getInstance();
	}
	void process(GameInfo &gameInfo)
	{
		string NoChangeCard[4] = { "狼人渗透者", "麻疯侏儒", "疯狂的科学家", "鬼灵爬行者" };
		switch (gameInfo.state)
		{
		case STATE_STARTGAME:
			cout << "游戏阶段：开始游戏,点击开始按钮，延时1秒" << endl;
			controlMouse->touchPosition(800, 600);//点击开始
			Sleep(1000);
			gameInfo.state = STATE_CHANGECARDSTART;
			break;
		case STATE_CHANGECARDSTART:
			cout << "游戏阶段：换牌阶段开始，识别卡牌数：" << gameInfo.currentNum << endl;
			if (gameInfo.currentNum < 3) break;
			if (gameInfo.currentNum == 3) gameInfo.first = true;
			else gameInfo.first = false;
			
			for (int i = 0; i < gameInfo.currentNum; ++i)
			{
				int flag = false;
				for (int j = 0; j < 4; ++j)
				{
					if (gameInfo.handCard[i].name == NoChangeCard[j])
					{
						flag = true;
					}
				}
				if (!flag)
				{
					cout << "更换卡牌,延时0.1秒：" << gameInfo.handCard[i].name << endl;
					controlMouse->touchPosition(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
					Sleep(100);
				}
			}
			gameInfo.state = STATE_CHANGECARDEND;
			break;
		case STATE_CHANGECARDEND:
			cout << "游戏阶段：换牌阶段结束，点击确定按钮，延时3秒" << endl;
			//确定
			controlMouse->touchPosition(500, 600);
			Sleep(3000);
			gameInfo.state = STATE_FIGHTSTART;
			break;
		case STATE_FIGHTSTART:
			cout << "游戏阶段：开始战斗阶段，延时10秒" << endl;
			Sleep(10000);//等待发牌结束
			gameInfo.state = STATE_OTHERTURN;
			break;
		case STATE_SELFTURN_PLAY:
			cout << "游戏阶段：自己出牌阶段，延时2秒" << endl;
			Sleep(2000);
			gameInfo.currentSpend++;
			gameInfo.couldUseSpend = gameInfo.currentSpend;
			for (int i = 0; i < gameInfo.currentNum; ++i)//最简单的AI，有啥出啥
			{
				if (gameInfo.handCard[i].behavior == "下场" && gameInfo.handCard[i].spend <= gameInfo.couldUseSpend)
				{
					cout << "ai:出牌：费用" << gameInfo.handCard[i].spend << "可用费用：" << gameInfo.couldUseSpend << "位置：" << gameInfo.handCard[i].x << " " << gameInfo.handCard[i].y << endl;
					controlMouse->playCard(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
					gameInfo.couldUseSpend -= gameInfo.handCard[i].spend;
					Sleep(100);
				}
			}

			/*for (int i = 0; i < 15; ++i)
			{
				controlMouse->playCardToPlayer(140+60*i,460);
				Sleep(500);
			}*/
		//	controlMouse->touchPosition(900, 350);//点击回合结束
			gameInfo.state = STATE_SELFTURN_FIGHT;
			
			controlMouse->moveToPosition(1000, 700);
			Sleep(1000);
			break;
		case STATE_SELFTURN_FIGHT:
			cout << "游戏阶段：攻击随从阶段" << endl;
			for (int i = 0; i < gameInfo.selfMonsterNum; ++i)
			{
				bool flag = false;
				for (int j = 0; j < gameInfo.otherMonsterNum; ++j)
				{
					if (gameInfo.otherMonster[j].taugh)
					{
						flag = true;
						controlMouse->fightMonster(gameInfo.selfMonster[i].x, gameInfo.selfMonster[i].y, gameInfo.otherMonster[j].x, gameInfo.otherMonster[j].y);
						cout << gameInfo.selfMonster[i].name << "攻击，对象位置：" << gameInfo.otherMonster[j].x<<gameInfo.otherMonster[j].y << endl;
						break;
					}
				}
				if (flag)
				{
					cout << "重新扫描对方场上随从" << endl;
					ProcessImage *processImage = ProcessImage::getInstance();
					Mat src = processImage->getCurrentImage();
					Mat otherFloor;
					src(Rect(204, 229, 630, 118)).copyTo(otherFloor);
					processImage->recoOtherMonster(otherFloor, gameInfo);
				}
				else
				{
					controlMouse->playCardToPlayer(gameInfo.selfMonster[i].x, gameInfo.selfMonster[i].y);
					cout << gameInfo.selfMonster[i].name << "攻击玩家" << endl;
				}
			}
			
	
			controlMouse->touchPosition(650, 620);//使用英雄技能
			gameInfo.state = STATE_OTHERTURN;//改变游戏状态
			controlMouse->touchPosition(900, 350);//点击回合结束
			
			controlMouse->moveToPosition(1000, 700);//移动到场景外，防止干扰
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