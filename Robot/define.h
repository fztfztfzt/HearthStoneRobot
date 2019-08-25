#pragma once

#include <vector>
#include <string>
#include <Windows.h>
#include <algorithm>
#include "opencv2/core/types.hpp"

enum STATE
{
	STATE_STARTGAME,
	STATE_CHANGECARDSTART,
	STATE_CHANGECARDEND,
	STATE_FIGHTSTART,
	STATE_SELFTURN_PLAY,
	STATE_SELFTURN_FIGHT,
	STATE_OTHERTURN,
	STATE_GAMEOVE
};
//const int OutsideX = 1500;
//const int OutsideY = 1000;
const int EndTurnX = 1300;
const int EndTurnY = 412;
const int HeroSkillX = 650;
const int HeroSkillY = 620;
const int OutSideX = 2000;
const int OutSideY = 1500;


const int HearthWindowWidth = 1600;
const int HearthWindowHeight = 900;
#define CARDGAME "Hearthstone"
#define CARDSINFO "HS/Magic1/"
#define CARDSDESCRPIT "HS/Magic1/cardInfo.txt"
//std::string CARDSDES = "HS/Magic1/cardInfo.txt";
const cv::Rect confirmButton(735, 675, 145, 78);
const cv::Rect endButton(1262, 420, 119, 43);
// 98 15?
#define MONSTERWIDTH 100
#define MONSTERSPAN 17
#define MONSTERHEIGHT 135
#define MONSTERCARDPOPWIDTH 235
#define MONSTERCARDPOPHEIGHT 326


const cv::Rect selfFloorRect(349, 418, 876, 141);
const cv::Rect otherFloorRect(349, 271, 876, 141);