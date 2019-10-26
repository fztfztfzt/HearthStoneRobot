#include <opencv2\opencv.hpp>
#include "processImage.h"
#include "gameInfo.h"
#include "ai.h"
#include "OutputInfo.h"

int main()
{
	COutputInfo outputInfo;
	ProcessImage *processImage = ProcessImage::getInstance();
	GameInfo gameInfo;
	AI aiControl;
	aiControl.startGame(gameInfo);
	//gameInfo.state = STATE_SELFTURN_FIGHT;
	while (true)
	{
		processImage->process(gameInfo);
		aiControl.process(gameInfo);
		Sleep(1000);
	}

	system("pause");
}