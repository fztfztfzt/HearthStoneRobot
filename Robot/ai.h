#pragma once
#include "define.h"
#include "gameInfo.h"
#include "ControlMouse.h"
#include "processImage.h"
#include <algorithm>
bool compareSpend(Card a, Card b)
{
	return a.spend < b.spend;
}

bool compareAttribute(Card a, Card b) {
	return (a.life + a.attack) > (b.life + b.attack);
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
		cout << "游戏开始：开始新的一轮练习模式,选择法师，点击开始按钮，延时1秒" << endl;

		//练习模式：
		//Sleep(1000);
		//controlMouse->touchPosition(810, 350); // 点击冒险模式
		//Sleep(1000);//延时1s
		//controlMouse->touchPosition(1149, 223); // 点击Normal模式
		//controlMouse->touchPosition(1164,771);//点击选择按钮
		controlMouse->touchPosition(594, 632);//选择法师
		controlMouse->touchPosition(1164, 760);//点击Play按钮
		//controlMouse->touchPosition(1187, 193); // 选择猎人
		controlMouse->touchPosition(1187, 222); // 选择战士
		controlMouse->touchPosition(1164, 760); //点击Play按钮
		Sleep(6000);//延时6s
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
		std::vector<string> NoChangeCards = { "淡水鳄", "鱼人袭击者", "血沼迅猛龙", "工程师学徒", "森金持盾卫士", "冰风雪人"};

		for (int i = 0; i < gameInfo.currentNum; ++i)
		{
			int bShouldChange = true;
			
			for (auto noChangeCard : NoChangeCards)
			{
				if (gameInfo.handCard[i].name == noChangeCard)
				{
					bShouldChange = false;
					break;
				}
			}
			if (bShouldChange)
			{
				cout << "更换卡牌：" << gameInfo.handCard[i].name << endl;
				controlMouse->touchPosition(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
			}
		}
		gameInfo.state = STATE_CHANGECARDEND;
	}
	
	void process(GameInfo &gameInfo)
	{
		Card handCardCopy[10];
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
			controlMouse->touchPosition((confirmButton.x + confirmButton.width/2), (confirmButton.y + confirmButton.height/2));
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
			std::sort(gameInfo.handCard, gameInfo.handCard + gameInfo.currentNum, compareSpend);
			for (int i = 0; i < gameInfo.currentNum; ++i) {
				handCardCopy[i] = gameInfo.handCard[i];
			}
			std::sort(handCardCopy, handCardCopy + gameInfo.currentNum, compareAttribute);
			for (int i = 0; i < gameInfo.currentNum; ++i)
			{
				if (!gameInfo.first && handCardCopy[i].behavior == "下场" && handCardCopy[i].spend == gameInfo.couldUseSpend + 1)
				{
					controlMouse->playCard(gameInfo.handCard[0].x, gameInfo.handCard[0].y);//幸运币
					gameInfo.currentNum -= 1;
					gameInfo.first = true;//幸运币出掉
					++gameInfo.couldUseSpend;
					cout << "下幸运币，重新识别手牌位置" << endl;
					return;
				}
				if (handCardCopy[i].behavior == "下场" && handCardCopy[i].spend <= gameInfo.couldUseSpend)
				{
					if (handCardCopy[i].type == "武器")
					{
						if (gameInfo.haveWeapon) continue;
						else gameInfo.haveWeapon = true;
					}
					cout << "ai:出牌：" << handCardCopy[i].name << " 费用" << handCardCopy[i].spend << " 可用费用：" << gameInfo.couldUseSpend << " 位置：" << handCardCopy[i].x << " " << handCardCopy[i].y << endl;
					controlMouse->playCard(handCardCopy[i].x, handCardCopy[i].y);
					gameInfo.couldUseSpend -= handCardCopy[i].spend;
					++havePlayNum;//已出牌数量+1，不包括幸运币
					if (gameInfo.couldUseSpend + !gameInfo.first < handCardCopy[i].spend) break;//不会出牌则不用重新扫描
					return;//重新扫描，确认手牌位置
				}
			}

			for (int i = 0; i < gameInfo.currentNum; ++i)
			{
				if (gameInfo.handCard[i].behavior == "敌人" && gameInfo.handCard[i].spend <= gameInfo.couldUseSpend)
				{
					cout << "ai:出牌：" << gameInfo.handCard[i].name << " 费用" << gameInfo.handCard[i].spend << " 可用费用：" << gameInfo.couldUseSpend << " 位置：" << gameInfo.handCard[i].x << " " << gameInfo.handCard[i].y << endl;
					controlMouse->playCardToPlayer(gameInfo.handCard[i].x, gameInfo.handCard[i].y);
					gameInfo.couldUseSpend -= gameInfo.handCard[i].spend;
					++havePlayNum;//已出牌数量+1，不包括幸运币
					if (gameInfo.couldUseSpend + !gameInfo.first < gameInfo.handCard[i].spend) break;//不会在出牌则不用重新扫描
					return;//重新扫描，确认手牌位置
				}
			}

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
					//gameInfo.otherMonster[j].taugh = false; //TODO
					if (gameInfo.otherMonster[j].taugh)
					{
						flag = true;
						controlMouse->fightMonster(gameInfo.selfMonster[i].x, gameInfo.selfMonster[i].y, gameInfo.otherMonster[j].x, gameInfo.otherMonster[j].y);
						cout << "我方(" << gameInfo.selfMonster[i].name << "attack:" << gameInfo.selfMonster[i].attack << "life:" << gameInfo.selfMonster[i].life << ")进行攻击 :" << gameInfo.otherMonster[j].attack << "位置：" << gameInfo.otherMonster[j].x << gameInfo.otherMonster[j].y << endl;
						if (gameInfo.selfMonster[i].life < gameInfo.otherMonster[j].attack)
						{
							cout << "我方进攻随从死亡，重新扫描场上随从" << endl;
							return;
						}
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
			Sleep(1000);
			break;
		case STATE_OTHERTURN:
			cout << "游戏阶段：对手出牌阶段" << endl;
			break;
		case STATE_GAMEOVE:
			cout << "游戏结束" << endl;
			controlMouse->touchPosition(OutSideX, OutSideY);//点击回合结束
			break;
		default:
			break;
		}
		//controlMouse->moveToPosition(OutsideX, OutsideY);
	}
};