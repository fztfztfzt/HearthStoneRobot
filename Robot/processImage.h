/*
ʹ��opencv���ͼ����д���
*/
#pragma once
#include <iostream>
#include <fstream>
#include <time.h>
#include <opencv2\opencv.hpp>
#include "opencv2/imgproc/types_c.h"
#include "opencv2/videoio/legacy/constants_c.h"    //BruteForceMatcherʵ���ڸ�ͷ�ļ���  
#include "opencv2/core/base.hpp"
#include "gameInfo.h"
#include "ControlMouse.h"
#include "handWritten-digit-recognition-based-on-OpenCV/handWritten digit recognition based on OpenCV/recognize.h"


//int num[11][9] = { { 0, 1, 1, 1, 0, 1, 1, 1, 1 }, { 0, 1, 1, 0, 1, 1, 0, 1, 1 }, { 1, 0, 1, 1, 0, 1, 0, 1, 0 }, { 0, 1, 0, 0, 1, 1, 0, 0, 1 },
//{ 0, 0, 1, 0, 0, 1, 0, 1, 1 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 1, 0, 1, 1, 1, 1 }, { 1, 1, 1, 0, 1, 0, 0, 1, 0 },
//{ 0, 0, 1, 0, 1, 1, 1, 0, 0 }, { 1, 1, 1, 1, 0, 1, 0, 0, 0 }, { 0, 1, 1, 1, 0, 1, 1, 1, 0 } };

class ProcessImage
{
	std::vector<Card> allCards;
	
	HWND hWnd;
	VideoWriter outputVideo;
	static ProcessImage* self;
	int gameOverNum = 0;
	ProcessImage();

	Mat selfFloorBG;
	Mat otherFloorBG;
	Mat weaponBG;
	Mat gameOverOtherBG;
	Mat gameOverSelfBG;
	Mat startBG;
public:
	static ProcessImage* getInstance();
	void initVideo(Mat &src, GameInfo &gameInfo);
	void saveVideo(Mat &src, GameInfo &gameInfo);
	void releaseVideo();
	void isGameOver(GameInfo &gameInfo);
	void gameOver(Mat& src, GameInfo& gameInfo);
	
	void process(GameInfo &gameInfo);
	void fightStart(Mat &src);
	void changeCardStart(Mat src, GameInfo &gameInfo);//Ԥ�����ƽ׶�ͼ��
	int getNumByImage(Mat src);
	int getNumByFullCard(Mat src);//��ȡ���ֲ�������
	int recoNum(Mat src);//����ʶ��
	void recoHandCrad(Mat src, GameInfo &gameInfo);
	void recoSelfMonster(Mat src, GameInfo &gameInfo);
	void recoOtherMonster(Mat src, GameInfo &gameInfo);
	bool isTaunt(Mat src);
	
	int recoImageNum(Mat src);
	void selfTurn(Mat src, GameInfo &gameInfo);//������ƽ׶γ���
	void selfTurnFight(Mat src, GameInfo &gameInfo);
	char* CaptureAnImage(HWND hWnd);
	Mat getCurrentImage();
	void otherTurn(Mat src, GameInfo &gameInfo);
	float compareImageBySub(Mat &a, Mat &a2);
	int compareImage(Mat &a, Mat &b);
	int compareImageByORB(Mat img_1);
	
private:
	const int MAXV = 1;
	
};

