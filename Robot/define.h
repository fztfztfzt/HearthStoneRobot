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
const int OutsideX = 1500;
const int OutsideY = 1000;
const int EndTurnX = 950;
const int EndTurnY = 380;
const int HeroSkillX = 650;
const int HeroSkillY = 620;
const int OutSideX = 1000;
const int OutSideY = 700;


const int HearthWindowWidth = 1600;
const int HearthWindowHeight = 900;
#define CARDGAME "Hearthstone"
#define CARDSINFO "HS/Magic1/"
#define CARDSDESCRPIT "HS/Magic1/cardInfo.txt"
//std::string CARDSDES = "HS/Magic1/cardInfo.txt";
const cv::Rect confirmButton(735, 675, 145, 78);