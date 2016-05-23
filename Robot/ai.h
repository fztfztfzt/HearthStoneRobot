#pragma once
#include <Windows.h>
#include "gameInfo.h"
#include "ControlMouse.h"
#include "define.h"
#include "processImage.h"
bool cmp(Card a, Card b)
{
	return a.spend < b.spend;
}
class AI
{
	HWND hWnd;
	ControlMouse *controlMouse;
	int havePlayNum;
public:
	AI(HWND h) :hWnd(h){
		controlMouse = ControlMouse::getInstance();
	}
	void startGame(GameInfo &gameInfo)
	{
		cout << "游戏阶段：开始新的一轮练习模式,选择术士，点击开始按钮，延时1秒" << endl;
		//练习模式：
		controlMouse->touchPosition(835,655);//点击选择按钮
		controlMouse->touchPosition(845, 495);//选择术士
		controlMouse->touchPosition(850, 645);//点击开始按钮
		Sleep(1000);//延时1s
		gameInfo.state = STATE_CHANGECARDSTART;
	}
	void changeCardStart(GameInfo &gameInfo)
	{
		cout << "游戏阶段：换牌阶段开始" << endl;
		cout << "识别卡牌数：" << gameInfo.currentNum << endl;
		if (gameInfo.currentNum == 3)
		{
			gameInfo.first = true;
		}
		else if (gameInfo.currentNum == 4)
		{
			gameInfo.first = false;
		}
		else
		{
			cout << "换牌个数识别错误" << endl;
			return;//识别错误
		}
		string NoChangeCard[4] = { "狼人渗透者", "麻疯侏儒", "疯狂的科学家", "鬼灵爬行者" };
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
				cout << "更换卡牌：" << gameInfo.handCard[i].name << endl;
				controlMouse->touchPosition(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
			}
		}
		gameInfo.state = STATE_CHANGECARDEND;
	}
	
	void process(GameInfo &gameInfo)
	{
		switch (gameInfo.state)
		{
		case STATE_STARTGAME:
			startGame(gameInfo);
			break;
		case STATE_CHANGECARDSTART:
			changeCardStart(gameInfo);			
			break;
		case STATE_CHANGECARDEND:
			cout << "游戏阶段：换牌阶段结束，点击确定按钮" << endl;
			//确定
			controlMouse->touchPosition(500, 600);
			gameInfo.state = STATE_FIGHTSTART;
			if(gameInfo.first) Sleep(15000);//等待发牌结束
			else Sleep(10000);
			break;
		case STATE_FIGHTSTART:
			cout << "游戏阶段：开始战斗阶段，延时10秒" << endl;
			Sleep(2000);
			gameInfo.state = STATE_OTHERTURN;
			break;
		case STATE_SELFTURN_PLAY:
			cout << "游戏阶段：自己出牌阶段" << endl;
			cout << "当前费用：" << gameInfo.currentSpend << " 可用费用:" << gameInfo.couldUseSpend << endl;
			
			//将手牌按费用排序
			sort(gameInfo.handCard, gameInfo.handCard + gameInfo.currentNum, cmp);
			/*for (int i = 0; i < gameInfo.currentNum; ++i)
			{
				cout << gameInfo.handCard[i].name;
			}*/
			//if (gameInfo.currentSpend == 1)//1费情况
			{
				
				for (int i = 0; i < gameInfo.currentNum; ++i)
				{
					if (!gameInfo.first && gameInfo.handCard[i].behavior == "下场" && gameInfo.handCard[i].spend == gameInfo.couldUseSpend + 1)
					{
						controlMouse->playCard(gameInfo.handCard[0].x, gameInfo.handCard[0].y);//幸运币
						gameInfo.currentNum -= 1;
						gameInfo.first = true;//幸运币出掉
						++gameInfo.couldUseSpend;
						cout << "下幸运币，重新识别手牌位置" << endl;
						return;
						
						//cout << "ai:出牌：费用" << gameInfo.handCard[i].spend << " 可用费用：" << gameInfo.couldUseSpend << " 位置：" << gameInfo.handCard[i].x << " " << gameInfo.handCard[i].y << endl;
						//controlMouse->playCard(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
						//gameInfo.couldUseSpend -= gameInfo.handCard[i].spend;
						//++havePlayNum;//已出牌数量+1，不包括幸运币
						//Sleep(100);
						//break;
					}
					if (gameInfo.handCard[i].behavior == "下场" && gameInfo.handCard[i].spend <= gameInfo.couldUseSpend)
					{
						if (gameInfo.handCard[i].type == "武器")
						{
							if (gameInfo.haveWeapon) continue;
							else gameInfo.haveWeapon = true;
						}
						cout << "ai:出牌：" << gameInfo.handCard[i].name<<" 费用" << gameInfo.handCard[i].spend << " 可用费用：" << gameInfo.couldUseSpend << " 位置：" << gameInfo.handCard[i].x << " " << gameInfo.handCard[i].y << endl;
						controlMouse->playCard(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
						gameInfo.couldUseSpend -= gameInfo.handCard[i].spend;
						++havePlayNum;//已出牌数量+1，不包括幸运币
						if (gameInfo.couldUseSpend + !gameInfo.first < gameInfo.handCard[i].spend) break;//不会在出牌则不用重新扫描
						return;//重新扫描，确认手牌位置
						
					}
				
					
					
				}

				for (int i = 0; i < gameInfo.currentNum; ++i)
				{
					if (gameInfo.handCard[i].behavior == "敌人" && gameInfo.handCard[i].spend <= gameInfo.couldUseSpend)
					{
						cout << "ai:出牌：" << gameInfo.handCard[i].name<<" 费用" << gameInfo.handCard[i].spend << " 可用费用：" << gameInfo.couldUseSpend << " 位置：" << gameInfo.handCard[i].x << " " << gameInfo.handCard[i].y << endl;
						controlMouse->playCardToPlayer(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
						gameInfo.couldUseSpend -= gameInfo.handCard[i].spend;
						++havePlayNum;//已出牌数量+1，不包括幸运币
						if (gameInfo.couldUseSpend + !gameInfo.first < gameInfo.handCard[i].spend) break;//不会在出牌则不用重新扫描
						return;//重新扫描，确认手牌位置
					}
				}
			}
			
			//for (int i = 0; i < gameInfo.currentNum; ++i)//最简单的AI，有啥出啥
			//{
			//	if (gameInfo.handCard[i].behavior == "下场" && gameInfo.handCard[i].spend!=0 && gameInfo.handCard[i].spend <= gameInfo.couldUseSpend)
			//	{
			//		cout << "ai:出牌：费用" << gameInfo.handCard[i].spend << " 可用费用：" << gameInfo.couldUseSpend << " 位置：" << gameInfo.handCard[i].x << " " << gameInfo.handCard[i].y << endl;
			//		controlMouse->playCard(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
			//		gameInfo.couldUseSpend -= gameInfo.handCard[i].spend;
			//		Sleep(100);
			//	}
			//}

			gameInfo.state = STATE_SELFTURN_FIGHT;
			
			controlMouse->moveToPosition(OutSideX, OutSideY);
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
						cout << "重新扫描场上随从" << endl;
						return;
					}
				}
				if (!flag)
				{
					controlMouse->playCardToPlayer(gameInfo.selfMonster[i].x, gameInfo.selfMonster[i].y);
					cout << gameInfo.selfMonster[i].name << "攻击玩家" << endl;
				}
			}
			
	
			controlMouse->touchPosition(HeroSkillX, HeroSkillY);//使用英雄技能
			gameInfo.state = STATE_OTHERTURN;//改变游戏状态
			gameInfo.currentSpend++;
			gameInfo.couldUseSpend = gameInfo.currentSpend;
			havePlayNum = 0;
			controlMouse->touchPosition(EndTurnX, EndTurnY);//点击回合结束
			
			controlMouse->moveToPosition(OutSideX, OutSideY);//移动到场景外，防止干扰
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