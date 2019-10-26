#include "processImage.h"


using namespace cv;
using namespace std;

ProcessImage* ProcessImage::self = NULL;

vector<vector<vector<bool>>> numModel;

bool contoursCmp(std::vector<Point> a, std::vector<Point> b)
{
	return a[0].x < b[0].x;
}

inline ProcessImage::ProcessImage() {
	numModel.resize(11);//��ȡ����ģ��
	ifstream f;
	f.open("numModel.txt", ios::in);

	bool b;
	char c;
	for (int i = 0; i < 11; ++i)
	{
		vector<vector<bool>> v;
		while (1)
		{
			vector<bool> vb;
			for (int j = 0; j < 9; ++j)
			{
				f >> b >> c;
				cout << b << c;
				vb.push_back(b);
			}
			cout << endl;
			v.push_back(vb);
			if (c != '#') break;
		}
		numModel[i] = v;
	}
	f.close();
	f.open(CARDSDESCRPIT, ios::in);//��ȡ������Ϣ
	while (!f.eof())
	{
		Card temp;
		f >> temp.name;
		if (temp.name[0] == '#')
		{
			char c[255];
			f.getline(c, 255);
			continue;
		}
		f >> temp.type >> temp.spend >> temp.attack >> temp.life >> temp.behavior >> temp.explain;
		cout << temp.name << " " << temp.type << " " << temp.spend << " " << temp.attack << " " << temp.life << " " << temp.behavior << " " << temp.explain << endl;
		allCards.push_back(temp);
	}
	f.close();

	hWnd = ::FindWindow("UnityWndClass", CARDGAME);
}

ProcessImage * ProcessImage::getInstance()
{
	if (self == NULL)
	{
		self = new ProcessImage();
	}
	return self;
}

void ProcessImage::initVideo(Mat &src, GameInfo &gameInfo)
{
	return;
	string fname = "HS/video/" + std::to_string(gameInfo.currentTimes) + ".avi";
	gameInfo.currentTimes++;
	outputVideo.open(fname, CV_FOURCC('M', 'J', 'P', 'G'), 1, Size(src.cols, src.rows), true);
}

void ProcessImage::saveVideo(Mat &src, GameInfo &gameInfo)
{
	return;
	outputVideo << src;
}

void ProcessImage::releaseVideo()
{
	outputVideo.release();
}

void ProcessImage::isGameOver(GameInfo &gameInfo)
{
	ControlMouse *cm = ControlMouse::getInstance();
	cm->moveToPosition(OutSideX, OutSideY);//�ƶ��������⣬��ֹ����
	Sleep(1000);
	Mat src = getCurrentImage();
	Mat sub, sub2;
	src(Rect(460, 75, 110, 120)).copyTo(sub);
	src(Rect(455, 515, 110, 120)).copyTo(sub2);

	double b = compareImageBySub(gameOverOtherBG, sub);
	double b2 = compareImageBySub(gameOverSelfBG, sub2);
	cout << "b:" << b << endl;
	cout << "b2:" << b2 << endl;
	if (b < 0.1 || b2 < 0.1)
	{
		++gameOverNum;
		Sleep(1000);
	}
	else
	{
		gameOverNum = 0;
	}
	if (gameOverNum > 3)
	{
		gameInfo.state = STATE_GAMEOVE;
		cout << "��Ϸ����" << endl;
		/*imshow("1", sub2);
		imshow("2", sub);
		waitKey(0);*/
	}
}

void ProcessImage::gameOver(Mat& src, GameInfo& gameInfo)
{
	Mat sub;
	float f;
	saveVideo(src, gameInfo);
	src(confirmButton).copyTo(sub);
	startBG = imread("HS/startBG.png");
	f = compareImageBySub(startBG, sub);
	cout << f << endl;
	if (f > 0.9)
	{
		cout << "��ʼ�µ�һ��" << endl;
		releaseVideo();
		gameInfo.currentTimes += 1;
		gameInfo.state = STATE_STARTGAME;
	}
}

void ProcessImage::process(GameInfo &gameInfo)
{
	Mat src = getCurrentImage();
	switch (gameInfo.state)
	{
	case STATE_STARTGAME:
		initVideo(src, gameInfo);
		break;
	case STATE_CHANGECARDSTART:
		saveVideo(src, gameInfo);
		changeCardStart(src, gameInfo);
		break;
	case STATE_FIGHTSTART:
		saveVideo(src, gameInfo);
		fightStart(src);
		break;
	case STATE_SELFTURN_PLAY:
		saveVideo(src, gameInfo);
		selfTurn(src, gameInfo);
		break;
	case STATE_SELFTURN_FIGHT:
		saveVideo(src, gameInfo);
		selfTurnFight(src, gameInfo);
		break;
	case STATE_OTHERTURN:
		saveVideo(src, gameInfo);
		otherTurn(src, gameInfo);
		isGameOver(gameInfo);
		break;
	case STATE_GAMEOVE:
		gameOver(src, gameInfo);
		break;
	default:
		break;
	}
}

void ProcessImage::fightStart(Mat &src)
{
	return;
	//��ȡ����
	src(selfFloorRect).copyTo(selfFloorBG);
	imwrite("HS/self_floor.png", selfFloorBG);
	src(otherFloorRect).copyTo(otherFloorBG);
	imwrite("HS/other_floor.png", otherFloorBG);
	src(Rect(337, 534, 112, 110)).copyTo(weaponBG);
	imwrite("HS/weaponBG.png", weaponBG);
	src(Rect(460, 75, 110, 120)).copyTo(gameOverOtherBG);
	imwrite("HS/gameOverOtherBG.png", gameOverOtherBG);
	src(Rect(455, 515, 110, 120)).copyTo(gameOverSelfBG);
	imwrite("HS/gameOverSelfBG.png", gameOverSelfBG);
}

void ProcessImage::changeCardStart(Mat src, GameInfo &gameInfo) //Ԥ�����ƽ׶�ͼ��
{
	Mat sure;
	src(confirmButton).copyTo(sure);
	Mat sureBG = imread("HS/sure.png");
	float ratio = compareImageBySub(sure, sureBG);
	cout << "��ȷ�ϰ�ť�����ԣ�" << ratio * 100 << "%" << endl;
	/*imshow("1", sure);
	imshow("2", sureBG);
	waitKey(0);*/
	if (ratio < 0.5) return;//֪����ȷ�ϰ�ť������������ʱ�Ž�����һ��
	for (int i = 0; i < src.rows; i++)//��ֵ������ɫ���£������Ϊ��ɫ
	{
		for (int j = 0; j < src.cols; j++)
		{
			uchar r = src.at<Vec3b>(i, j)[0];
			uchar g = src.at<Vec3b>(i, j)[1];
			uchar b = src.at<Vec3b>(i, j)[2];
			if (r < 30 && g >= 70 && g <= 140 && b < 30)
			{
				src.at<Vec3b>(i, j)[0] = 0;
				src.at<Vec3b>(i, j)[1] = 0;
				src.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	//imshow("�Ҷ�",gray);
	Mat thr;
	threshold(gray, thr, 50, 255, CV_THRESH_BINARY);
	//imshow("gray", thr);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(thr, thr, element);
	erode(thr, thr, element);
	//erode(thr, thr, element);
	//dilate(thr, thr, element);
	//imshow("fushi", thr);
	//waitKey(0);
	vector<std::vector<Point>> contours;
	findContours(thr,			//ͼ��
		contours,				//������
		//����ͼ�����˽ṹ����Ϣ����ѡ����������û��ѡ��
		CV_RETR_EXTERNAL,			//��ȡ�����ķ����������ȡ��Χ������
		CV_CHAIN_APPROX_NONE);		//�������Ƶķ��������ﲻ���ƣ���ȡȫ��������
	//��ӡ������Ϣ
	//std::cout << "������Χ������" << contours.size() << "��" << std::endl;
	//std::vector<std::vector<Point>>::const_iterator itContours = contours.begin();
	/*for (; itContours != contours.end(); ++itContours)
	{
		std::cout << "ÿ�������ĳ���: " << itContours->size() << std::endl;
	}*/

	Mat result(thr.size(), CV_8U, Scalar(255));
	result.setTo(Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 2);
	//imshow("��ȡ��Χ����", result);

	//��ȥ̫������̫�̵�����
	int cmin = 700;
	int cmax = 1000;
	std::vector<std::vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if (itc->size() < cmin || itc->size() > cmax)
			itc = contours.erase(itc);
		else
			++itc;
	}

	//��������

	//��������������Ϊ�����壬����������ָʾ�����ﻭ����������������ɫ���ߴ�
	//	drawContours(result, contours, -1, Scalar(0), 2);
	//�������ػ��ڰװ���
	result.setTo(Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 1);
	//imshow("ȥ����С��", result); //��Ҫע����ȡ�����ķ������кܶ��֣�����CV_RETR_LIST������������
	//waitKey(0);
	//��������״������
	//��Ӿ���
	Mat matio[10];
	int num = contours.size();
	gameInfo.currentNum = num;
	for (int i = 0; i < contours.size(); ++i)
	{
		Rect r0 = boundingRect(Mat(contours[i]));
		src(r0).copyTo(matio[i]);
		rectangle(src, r0, Scalar(0), 2);
		/*imshow("" + i, matio[i]);
		waitKey(0);*/
		//gameInfo.handCard[i].spend = getNumByFullCard(matio[i]);

		int cardID = compareImageByORB(matio[i]);
		gameInfo.handCard[i] = allCards[cardID];
		gameInfo.handCard[i].x = r0.x + r0.width / 2;
		gameInfo.handCard[i].y = r0.y + r0.height / 2;
		cout << "���ƽ׶Σ�ʶ����Ŀ�����Ϣ�����ƣ�" << gameInfo.handCard[i].name << " ���ã�" << gameInfo.handCard[i].spend << " λ��:" << gameInfo.handCard[i].x << " " << gameInfo.handCard[i].y << endl;
	}
	//	imshow("��Ӿ���", src);
}

int ProcessImage::getNumByImage(Mat src)
{
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	Mat thr;
	threshold(gray, thr, 250, 255, CV_THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat cz;
	dilate(thr, cz, element);
	erode(cz, cz, element);
	//	imshow("gray2", cz);
	//	waitKey(0);
	vector<std::vector<Point>> contours;
	findContours(cz,			//ͼ��
		contours,				//������
		//����ͼ�����˽ṹ����Ϣ����ѡ����������û��ѡ��
		CV_RETR_EXTERNAL,			//��ȡ�����ķ����������ȡ��Χ������
		CV_CHAIN_APPROX_NONE);		//�������Ƶķ��������ﲻ���ƣ���ȡȫ��������
	//��ӡ������Ϣ
	/*std::cout << "������Χ������" << contours.size() << "��" << std::endl;
	for (int i = 0; i < contours.size(); ++i)
	{
		cout << contours[i].size() << endl;;
	}*/
	Rect r0 = boundingRect(Mat(contours[contours.size() - 1]));
	Mat matio;
	thr(r0).copyTo(matio);
	rectangle(src, r0, Scalar(0), 2);
	//	imshow("222", matio);
	//	waitKey(0);
	return recoNum(matio);
}

int ProcessImage::getNumByFullCard(Mat src) //��ȡ���ֲ�������
{
	Mat image;
	src(Rect(0, 0, src.rows / 4, src.cols / 3)).copyTo(image);
	return getNumByImage(image);
}

int ProcessImage::recoNum(Mat src) //����ʶ��
{
	//int finalNum = handWritingImageRecognize(src);
	//return finalNum;
	Mat sub[9];
	int h = src.rows / 3;
	int w = src.cols / 3;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			src(Rect(j*w, i*h, w, h)).copyTo(sub[i * 3 + j]);
			/*imshow("sub", sub[i*3 + j]);
			waitKey(0);*/
		}
	}
	bool temp[9] = { 0 };
	for (int i = 0; i < 9; ++i)
	{
		int num = 0;
		for (int j = 0; j < sub[i].cols; ++j)
		{
			for (int k = 0; k < sub[i].rows; ++k)
			{
				if (sub[i].at<uchar>(k, j) == 255)
				{
					num++;
				}
			}
		}

		if (num > w*h / 2) temp[i] = 1, cout << "1" << ",";
		else cout << "0" << ",";
		//if (i % 3 == 2) cout << endl;
		//cout << num << " "<<endl;
	}
	int maxsum = 0, maxn = 0;
	for (int i = 0; i < 11; ++i)
	{
		for (int k = 0; k < numModel[i].size(); ++k)
		{
			int sum = 0;
			for (int j = 0; j < 9; ++j)
			{
				if (temp[j] == numModel[i][k][j])
				{
					++sum;
				}
			}
			if (sum > maxsum)
			{
				maxsum = sum;
				maxn = i;
			}
		}
	}
	cout << "ʶ����������ǣ�" << maxn << endl;
	return maxn;
}

void ProcessImage::recoHandCrad(Mat src, GameInfo &gameInfo)
{
	//ʶ������
	clock_t startT = clock();
	Mat image, src2;
	src(Rect(484, 778, 554, src.rows - 778)).copyTo(src2);
	src2.copyTo(image);
	for (int i = 0; i < src2.rows; i++)//��ֵ������ɫ���£������Ϊ��ɫ
	{
		for (int j = 0; j < src2.cols; j++)
		{
			uchar r = src2.at<Vec3b>(i, j)[0];
			uchar g = src2.at<Vec3b>(i, j)[1];
			uchar b = src2.at<Vec3b>(i, j)[2];
			if (r == 255 && g == 255 && b == 255)
			{
				image.at<Vec3b>(i, j)[0] = 255;
				image.at<Vec3b>(i, j)[1] = 255;
				image.at<Vec3b>(i, j)[2] = 255;
			}
			else
			{
				image.at<Vec3b>(i, j)[0] = 0;
				image.at<Vec3b>(i, j)[1] = 0;
				image.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);//ת��Ϊ��ͨ�����ڽ���������ȡ
	//imshow("gray", gray);

	//Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));//��ʴ����Ч������
	//dilate(thr, thr, element);
	//erode(thr, thr, element);
	//dilate(thr, thr, element);
	//erode(thr, thr, element);
	//imshow("fushi", thr);
	vector<std::vector<Point>> contours;
	findContours(gray,			//ͼ��
		contours,				//������
		//����ͼ�����˽ṹ����Ϣ����ѡ����������û��ѡ��
		CV_RETR_EXTERNAL,			//��ȡ�����ķ����������ȡ��Χ������
		CV_CHAIN_APPROX_NONE);		//�������Ƶķ��������ﲻ���ƣ���ȡȫ��������
	double durationT = (double)(clock() - startT) / CLOCKS_PER_SEC;
	cout << "����ʶ��׶Σ���ȡ�����ֲ�����ʱ:" << durationT << "��" << endl;
	//��ӡ������Ϣ
	//std::cout << "������Χ������" << contours.size() << "��" << std::endl;
	//std::vector<std::vector<Point>>::const_iterator itContours = contours.begin();
	/*for (; itContours != contours.end(); ++itContours)
	{
	std::cout << "ÿ�������ĳ���: " << itContours->size() << std::endl;
	}*/

	Mat result(gray.size(), CV_8U, Scalar(255));
	result.setTo(Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 2);
	//imshow("��ȡ��Χ����", result);
	//waitKey(0);

	//��ȥ̫������̫�̵�����
	int cmin = 30;
	int cmax = 100;
	std::vector<std::vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		//cout << "�������ֲ���������С��" << itc->size()<<endl;
		if (itc->size() < cmin || itc->size() > cmax)
			itc = contours.erase(itc);
		else
			++itc;
	}

	//��������������Ϊ�����壬����������ָʾ�����ﻭ����������������ɫ���ߴ�
	//	drawContours(result, contours, -1, Scalar(0), 2);
	//�������ػ��ڰװ���
	//result.setTo(Scalar(255));
	//drawContours(result, contours, -1, Scalar(0), 1);
	//imshow("ȥ����С��", result); //��Ҫע����ȡ�����ķ������кܶ��֣�����CV_RETR_LIST������������
	//waitKey(0);
	//��������״������
	//��Ӿ���
	Mat matio;
	gameInfo.currentNum = 0;
	for (int i = 0; i < contours.size(); ++i)
	{
		//cout << contours[i].size() << endl;
		Rect r0 = boundingRect(Mat(contours[i]));
		if (r0.width >= r0.height) continue;
		src2(r0).copyTo(matio);
		//rectangle(src2, r0, Scalar(0), 2);
		/*imshow("iiiiiiiii", matio);
		waitKey(0);*/
		//gameInfo.handCard[gameInfo.currentNum].spend = getNumByImage(matio[i]);
		int x = 484 + r0.x + r0.width / 2 + 10;
		int y = 778 + r0.y + r0.height / 2 + 30;
		ControlMouse *con = ControlMouse::getInstance();
		con->moveToPosition(x, y);
		int thisCardWidth = 250;
		int thisCardHeight = min(350, HearthWindowHeight - y + 320 + 30);
		Rect r1(x - 100 - 10, y - 320 - 30, thisCardWidth, thisCardHeight);

		//cout << gameInfo.handCard[gameInfo.currentNum].x << " " << gameInfo.handCard[gameInfo.currentNum].y << endl;
		//cout << r1.x << " " << r1.y << " " << r1.width << " " << r1.height << endl;
		Mat image = getCurrentImage();
		//imshow("1111111111", image); waitKey(0);
		/*imshow("test1", image);
		waitKey(0);*/
		Mat image1;
		image(r1).copyTo(image1);
		/*imshow("test", image1);
		waitKey(0);*/
		clock_t start = clock();
		int cardID = compareImageByORB(image1);
		double duration = (double)(clock() - start) / CLOCKS_PER_SEC;
		cout << "�Ƚ�һ�ſ�����ʱ:" << duration << "��" << endl;
		gameInfo.handCard[gameInfo.currentNum] = allCards[cardID];
		gameInfo.handCard[gameInfo.currentNum].x = x;
		gameInfo.handCard[gameInfo.currentNum].y = y;
		cout << "���ƽ׶Σ�ʶ����Ŀ�����Ϣ�����ƣ�" << gameInfo.handCard[gameInfo.currentNum].name << " ���ã�" << gameInfo.handCard[gameInfo.currentNum].spend << " λ��:" << gameInfo.handCard[gameInfo.currentNum].x << " " << gameInfo.handCard[gameInfo.currentNum].y << endl;
		gameInfo.currentNum++;
	}
	//imshow("��Ӿ���", src);

	//	findContours(thr,			//ͼ�� 
	//	contours,				//������
	//	//����ͼ�����˽ṹ����Ϣ����ѡ����������û��ѡ��
	//	CV_RETR_LIST,			//��ȡ�����ķ����������ȡ����������
	//	CV_CHAIN_APPROX_NONE);		//�������Ƶķ��������ﲻ���ƣ���ȡȫ������
	////��������
	//drawContours(result, contours, -1, Scalar(0), 2);
	//imshow("��ȡ��������", result);
}

void ProcessImage::recoSelfMonster(Mat src, GameInfo &gameInfo)
{
	Mat floor = imread("HS/self_floor.png", 1);
	Mat out;
	Mat gray, floor_gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	cvtColor(floor, floor_gray, CV_BGR2GRAY);
	//imshow("yuan", src);
	//imshow("floor", floor);
	subtract(floor_gray, gray, out);
	cout << "��ǰ�����ȥ���汳���ɹ���" << endl;
	Mat thr;
	threshold(out, thr, 40, 255, CV_THRESH_BINARY);
	//imshow("test", thr);
	//waitKey(0);
	//Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));//��ʴ����Ч������
	//dilate(thr, thr, element);
	//erode(thr, thr, element);
	//dilate(thr, thr, element);
	//erode(thr, thr, element);
	//cout << "��ʴ���ͽ�����" << endl;
	/*imshow("thr", thr);
	waitKey(0);*/

	int bg = 0, ed = 0;
	for (int i = 0; i < thr.cols; ++i)
	{
		if (thr.at<uchar>(thr.rows / 2, i) == 255)
		{
			if (bg == 0) bg = i;
			else ed = i;
		}
	}
	cout << bg << " " << ed << endl;
	const int MonsterWidth = MONSTERWIDTH + MONSTERSPAN;
	int count = (ed - bg + MONSTERSPAN + 50) / MonsterWidth;
	gameInfo.selfMonsterNum = 0;
	for (int i = 0; i < count; ++i)
	{
		Rect r0(bg + i * MonsterWidth, 0, MonsterWidth, thr.rows);
		Mat temp;
		if (r0.x + r0.width > src.cols || r0.y + r0.height > src.rows) continue;
		src(r0).copyTo(temp);
		//imshow("tests", temp);
		//waitKey(0);
		int x = r0.x + r0.width / 2;
		int y = r0.y + r0.height / 2;
		x += 10 + selfFloorRect.x;
		y += 30 + selfFloorRect.y;

		ControlMouse *cm = ControlMouse::getInstance();
		cm->moveToPosition(x, y);
		Sleep(1000);
		Mat image = getCurrentImage();
		Mat sub;
		image(Rect(r0.x + r0.width + selfFloorRect.x + 3, r0.y + selfFloorRect.y - MONSTERCARDPOPHEIGHT / 2 + r0.height / 2, MONSTERCARDPOPWIDTH, MONSTERCARDPOPHEIGHT)).copyTo(sub);
		//imshow("sub",sub);
		//waitKey(0);
		int cardID = compareImageByORB(sub);
		gameInfo.selfMonster[gameInfo.selfMonsterNum] = allCards[cardID];
		gameInfo.selfMonster[gameInfo.selfMonsterNum].x = x;
		gameInfo.selfMonster[gameInfo.selfMonsterNum].y = y;

		//ʶ����ֵ
		Mat attackImage;
		temp(Rect(0, 0, temp.cols / 2, temp.rows)).copyTo(attackImage);//��ȡ�������
		gameInfo.otherMonster[gameInfo.selfMonsterNum].attack = recoImageNum(attackImage);
		Mat lifeImage;
		temp(Rect(temp.cols / 2, 0, temp.cols / 2, temp.rows)).copyTo(lifeImage);//��ȡ�������
		gameInfo.otherMonster[gameInfo.selfMonsterNum].life = recoImageNum(lifeImage);

		cout << "���ƽ׶Σ�ʶ�𼺷�������ӣ�������" << gameInfo.selfMonsterNum + 1 << " ���ƣ�" << gameInfo.selfMonster[gameInfo.selfMonsterNum].name << " ������" << gameInfo.otherMonster[gameInfo.selfMonsterNum].attack << " ������" << gameInfo.otherMonster[gameInfo.selfMonsterNum].life << " ���ã�" << gameInfo.selfMonster[gameInfo.selfMonsterNum].spend << " λ��:" << gameInfo.selfMonster[gameInfo.selfMonsterNum].x << " " << gameInfo.selfMonster[gameInfo.selfMonsterNum].y << endl;
		gameInfo.selfMonsterNum++;
	}

	//vector<std::vector<Point>> contours;
	//findContours(thr,			//ͼ��
	//	contours,				//������
	//	//����ͼ�����˽ṹ����Ϣ����ѡ����������û��ѡ��
	//	CV_RETR_EXTERNAL,			//��ȡ�����ķ����������ȡ��Χ������
	//	CV_CHAIN_APPROX_NONE);		//�������Ƶķ��������ﲻ���ƣ���ȡȫ��������
	//cout << "��ȡ����������" << endl;
	//int cmin = 300;
	//Mat matio;
	//gameInfo.selfMonsterNum = 0;
	//for (int i = 0; i < contours.size(); ++i)
	//{
	//	cout << "ʶ���Լ�������ӣ�" << contours[i].size() << endl;
	//	if ((int)contours[i].size() > cmin)
	//	{
	//		//cout <<contours[i].size() << endl;
	//		Rect r0 = boundingRect(Mat(contours[i]));
	//		//cout << r0.x << " " << r0.y << " " << r0.width << " " << r0.height << endl;
	//	//	if (r0.width >= r0.height) continue;
	//		src(r0).copyTo(matio);
	//		/*imshow("tests",matio);
	//		waitKey(0);*/
	//		int x = r0.x + r0.width / 2;
	//		int y = r0.y + r0.height / 2;
	//		x += 10 + 166;
	//		y += 30 + 363;
	//		ControlMouse *cm = ControlMouse::getInstance();
	//		cm->moveToPosition(x, y);
	//		Sleep(1000);
	//		Mat image = getCurrentImage();
	//		Mat sub;
	//		image(Rect(r0.x + r0.width+166+10, r0.y+363-r0.height/2-30, 205, 275)).copyTo(sub);
	//		/*imshow("sub",sub);
	//		waitKey(0);*/
	//		int cardID = compareImageByORB(sub);
	//		gameInfo.selfMonster[gameInfo.selfMonsterNum] = allCards[cardID];
	//		gameInfo.selfMonster[gameInfo.selfMonsterNum].x = x;
	//		gameInfo.selfMonster[gameInfo.selfMonsterNum].y = y;
	//		cout << "���ƽ׶Σ�ʶ�𼺷�������ӣ�������" << gameInfo.selfMonsterNum+1<<" ���ƣ�" << gameInfo.selfMonster[gameInfo.selfMonsterNum].name << " ���ã�" << gameInfo.selfMonster[gameInfo.selfMonsterNum].spend << " λ��:" << gameInfo.selfMonster[gameInfo.selfMonsterNum].x << " " << gameInfo.selfMonster[gameInfo.selfMonsterNum].y << endl;
	//		gameInfo.selfMonsterNum++;
	//		
	//	}
	//}
	cout << "ʶ���Լ�������ӽ�����" << endl;
}

void ProcessImage::recoOtherMonster(Mat src, GameInfo &gameInfo)
{
	Mat floor = imread("HS/other_floor.png", 0);
	Mat out;
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	//imshow("yuan", gray);
	//imshow("floor", floor);
	subtract(floor, gray, out);
	//imshow("test", out);
	//waitKey(0);
	Mat thr;
	threshold(out, thr, 50, 255, CV_THRESH_BINARY);
	//Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));//��ʴ����Ч������
	//dilate(thr, thr, element);
	//erode(thr, thr, element);
	//dilate(thr, thr, element);
	//erode(thr, thr, element);

	int sum = 0, bg = 0, ed = 0;
	for (int i = 0; i < thr.cols; ++i)
	{
		if (thr.at<uchar>(thr.rows / 2, i) == 255)
		{
			if (bg == 0) bg = i;
			else ed = i;
		}
	}
	cout << bg << " " << ed << endl;
	const int MonsterWidth = MONSTERWIDTH + MONSTERSPAN;
	int count = (ed - bg + MONSTERSPAN + 50) / MonsterWidth;
	gameInfo.otherMonsterNum = 0;
	for (int i = 0; i < count; ++i)
	{
		Rect r0(bg + i * MonsterWidth, 0, MonsterWidth, thr.rows);
		if (r0.x + r0.width > src.cols || r0.y + r0.height > src.rows) continue;
		int MonstersSpan = 0;
		if (i > 0) {
			MonstersSpan = (i - 1) * MONSTERSPAN;
		}
		Rect rcMonsterInner(bg + i * MONSTERWIDTH + MonstersSpan, 0, MONSTERWIDTH, MONSTERHEIGHT);
		Mat temp;
		src(rcMonsterInner).copyTo(temp);
		/*imshow("tests", temp);
		waitKey(0);*/
		int x = r0.x + r0.width / 2;
		int y = r0.height / 2;
		x += otherFloorRect.x + 10;
		y += otherFloorRect.y + 30;

		ControlMouse *cm = ControlMouse::getInstance();
		cm->moveToPosition(x, y);

		//imshow("1",temp);
		//waitKey(0);
		gameInfo.otherMonster[gameInfo.otherMonsterNum].taugh = isTaunt(temp);
		gameInfo.otherMonster[gameInfo.otherMonsterNum].x = x;
		gameInfo.otherMonster[gameInfo.otherMonsterNum].y = y;

		//ʶ����ֵ
		Mat attackImage;
		temp(Rect(0, 0, temp.cols / 2, temp.rows)).copyTo(attackImage);//��ȡ�������

		gameInfo.otherMonster[gameInfo.otherMonsterNum].attack = recoImageNum(attackImage);
		Mat lifeImage;
		temp(Rect(temp.cols / 2, 0, temp.cols / 2, temp.rows)).copyTo(lifeImage);//��ȡ�������
		gameInfo.otherMonster[gameInfo.otherMonsterNum].life = recoImageNum(lifeImage);

		cout << "���ƽ׶Σ��Է����Ͽ�����Ϣ���Ƿ񳰷�" << gameInfo.otherMonster[gameInfo.otherMonsterNum].taugh << " ������" << gameInfo.otherMonster[gameInfo.otherMonsterNum].attack << " ������" << gameInfo.otherMonster[gameInfo.otherMonsterNum].life << " λ��:" << gameInfo.otherMonster[gameInfo.otherMonsterNum].x << " " << gameInfo.otherMonster[gameInfo.otherMonsterNum].y << endl;
		gameInfo.otherMonsterNum++;
		/*imshow("sub",sub);
		waitKey(0);*/
	}

	//vector<std::vector<Point>> contours;
	//findContours(thr,			//ͼ��
	//	contours,				//������
	//	//����ͼ�����˽ṹ����Ϣ����ѡ����������û��ѡ��
	//	CV_RETR_EXTERNAL,			//��ȡ�����ķ����������ȡ��Χ������
	//	CV_CHAIN_APPROX_NONE);		//�������Ƶķ��������ﲻ���ƣ���ȡȫ��������

	//int cmin = 100;
	//Mat matio[10];
	//std::vector<std::vector<Point>>::const_iterator itc = contours.begin();
	//gameInfo.otherMonsterNum = 0;
	//for (int i = 0; i < contours.size(); ++i)
	//{
	//	cout << itc->size() << endl;
	//	if (itc->size() > cmin)
	//	{
	//		Rect r0 = boundingRect(Mat(contours[i]));
	//		if (r0.width >= r0.height) continue;
	//		src(r0).copyTo(matio[0]);
	//		imshow("tests",matio[0]);
	//		waitKey(0);
	//		int x = r0.width/2 ;
	//		int y = r0.height/2;
	//		x += 204;
	//		y +=  229+40;
	//		/*ControlMouse *cm = ControlMouse::getInstance();
	//		cm->moveToPosition(x, y);
	//		Sleep(1000);
	//		HWND hWnd = ::FindWindow("UnityWndClass", CARDGAME);
	//		Mat image = getCurrentImage(hWnd);
	//		Mat sub;
	//		image(Rect(x, y, 102, 126)).copyTo(sub);
	//		imshow("sub", sub);
	//		waitKey(0);*/
	//	
	//		gameInfo.otherMonster[gameInfo.otherMonsterNum].taugh = isTaunt(matio[0]);
	//		gameInfo.otherMonster[gameInfo.otherMonsterNum].x = x;
	//		gameInfo.otherMonster[gameInfo.otherMonsterNum].y = y;
	//		cout << "���ƽ׶Σ��Է����Ͽ�����Ϣ���Ƿ񳰷�" << gameInfo.otherMonster[gameInfo.otherMonsterNum].taugh << " λ��:" << gameInfo.otherMonster[gameInfo.otherMonsterNum].x << " " << gameInfo.otherMonster[gameInfo.otherMonsterNum].y << endl;
	//		gameInfo.otherMonsterNum++;
	//		/*imshow("sub",sub);
	//		waitKey(0);*/
	//	}
	//	++itc;
	//}
}

bool ProcessImage::isTaunt(Mat src)
{
	Mat src2, image;
	src.copyTo(src2);
	src.copyTo(image);
	//imshow("image", src);
	//waitKey(0);
	double sum = 0;
	for (int i = 0; i < src2.rows; i++)//��ֵ������ɫ���£������Ϊ��ɫ
	{
		for (int j = 0; j < src2.cols; j++)
		{
			uchar r = src2.at<Vec3b>(i, j)[0];
			uchar g = src2.at<Vec3b>(i, j)[1];
			uchar b = src2.at<Vec3b>(i, j)[2];
			if (r < 200 && g < 200 && b < 200 && r>40 && g>40 && b>40 && abs(r - g) < 25 && abs(r - b) < 25 && abs(g - b) < 25)
			{
				sum++;
				image.at<Vec3b>(i, j)[0] = 255;
				image.at<Vec3b>(i, j)[1] = 255;
				image.at<Vec3b>(i, j)[2] = 255;
			}
			else
			{
				image.at<Vec3b>(i, j)[0] = 0;
				image.at<Vec3b>(i, j)[1] = 0;
				image.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	cout << "sum:" << sum << "�� ��:" << src2.rows << " " << src2.cols << endl;
	bool bIsTaunt = (sum > 2500);
	if (bIsTaunt) {
		cout << "���ֳ���" << endl;
		//imshow("image", src);
		//waitKey(0);
	}
	return bIsTaunt;
}

int ProcessImage::recoImageNum(Mat src)
{
	//imshow("b", src);
	for (int i = 0; i < src.rows; i++)//��ֵ������ɫ���£������Ϊ��ɫ
	{
		for (int j = 0; j < src.cols; j++)
		{
			uchar b = src.at<Vec3b>(i, j)[0];
			uchar g = src.at<Vec3b>(i, j)[1];
			uchar r = src.at<Vec3b>(i, j)[2];
			//cout << (int)r << " " << (int)g << " " << (int)b << endl;
			if ((r >= 230 && g >= 230 && b >= 230) || (r == 0 && g == 255 && b == 0) || (r == 255 && g == 0 && b == 0))
			{
				src.at<Vec3b>(i, j)[0] = 255;
				src.at<Vec3b>(i, j)[1] = 255;
				src.at<Vec3b>(i, j)[2] = 255;
			}
			else
			{
				src.at<Vec3b>(i, j)[0] = 0;
				src.at<Vec3b>(i, j)[1] = 0;
				src.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);//ת��Ϊ��ͨ�����ڽ���������ȡ
	//imshow("a", gray);
	//waitKey(0);
	vector<std::vector<Point>> contours;
	findContours(gray,			//ͼ��
		contours,				//������
		//����ͼ�����˽ṹ����Ϣ����ѡ����������û��ѡ��
		CV_RETR_EXTERNAL,			//��ȡ�����ķ����������ȡ��Χ������
		CV_CHAIN_APPROX_NONE);		//�������Ƶķ��������ﲻ���ƣ���ȡȫ��������
	//��ӡ������Ϣ
	std::cout << "ʶ��ͼ����ֵ��������Χ������" << contours.size() << "��" << std::endl;
	std::vector<std::vector<Point>>::const_iterator itContours = contours.begin();
	for (; itContours != contours.end(); ++itContours)
	{
		std::cout << "ÿ�������ĳ���: " << itContours->size() << std::endl;
	}
	//��ȥ̫������̫�̵�����
	int cmin = 30;
	int cmax = 100;
	std::vector<std::vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		//cout << "�������ֲ���������С��" << itc->size() << endl;
		if (itc->size() < cmin || itc->size() > cmax)
			itc = contours.erase(itc);
		else
			++itc;
	}

	Mat result(gray.size(), CV_8U, Scalar(255));
	result.setTo(Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 2);
	//imshow("��ȡ��Χ����", result);
	//waitKey(0);
	Mat matio;
	int num = contours.size();
	int sum = 0;
	sort(contours.begin(), contours.begin() + contours.size(), contoursCmp);
	for (int i = 0; i < contours.size(); ++i)
	{
		Rect r0 = boundingRect(Mat(contours[i]));
		if (r0.width >= r0.height) continue;
		src(r0).copyTo(matio);
		rectangle(src, r0, Scalar(255), 2);
		//imshow("", matio);
		//waitKey(0);
		sum = sum * 10 + recoNum(matio);

	}
	return sum;
}

void ProcessImage::selfTurn(Mat src, GameInfo &gameInfo) //������ƽ׶γ���
{
	clock_t start = clock();
	recoHandCrad(src, gameInfo);
	double duration = (double)(clock() - start) / CLOCKS_PER_SEC;
	cout << "ʶ����������ʱ:" << duration << "��" << endl;
	cout << "�ж�Ӣ���Ƿ�������" << endl;
	clock_t start2 = clock();
	Mat weapon;
	src(Rect(337, 534, 112, 110)).copyTo(weapon);
	Mat weaponBG = imread("HS/weaponBG.png");
	double rs = compareImageBySub(weapon, weaponBG);
	/*imshow("wuqi", weapon);
	imshow("bg", weaponBG);

	waitKey(0);*/
	cout << "����ƥ��ȣ�" << (1 - rs) * 100 << "%" << endl;
	double duration2 = (double)(clock() - start2) / CLOCKS_PER_SEC;
	cout << "�ж��Ƿ���������ʱ:" << duration2 << "��" << endl;
	gameInfo.haveWeapon = false;
	if (rs < 0.9)
	{
		cout << "Ӣ��������:" << rs << endl;
		gameInfo.haveWeapon = true;
	}
	/*ControlMouse *controlMouse = ControlMouse::getInstance();
	controlMouse->moveToPosition(1000, 700);
	Sleep(1000);*/
	////ʶ���ѷ��������
	//Mat selfFloor;
	//src(Rect(166, 363, 680, 115)).copyTo(selfFloor);
	//recoSelfMonster(selfFloor, gameInfo);
	//
	////ʶ��Է��������
	//Mat otherFloor;
	//src(Rect(204, 229, 630, 118)).copyTo(otherFloor);
	//recoOtherMonster(otherFloor, gameInfo);
}

void ProcessImage::selfTurnFight(Mat src, GameInfo &gameInfo)
{
	cout << "ʶ���ѷ��������" << endl;
	Mat selfFloor;
	src(selfFloorRect).copyTo(selfFloor);
	recoSelfMonster(selfFloor, gameInfo);
	//cout << "�ж�Ӣ���Ƿ�������" << endl;
	//clock_t start = clock();
	//Mat weapon;
	//src(Rect(337, 534, 112, 110)).copyTo(weapon);
	//Mat weaponBG = imread("HS/weaponBG.png");
	//double rs = compareImageBySub(weapon, weaponBG);
	///*imshow("wuqi", weapon);
	//imshow("bg", weaponBG);
	//
	//waitKey(0);*/
	//cout << "����ƥ��ȣ�" << rs*100<<"%" << endl;
	//double duration = (double)(clock() - start) / CLOCKS_PER_SEC;
	//cout << "�ж��Ƿ���������ʱ:" << duration << "��" << endl;
	if (gameInfo.haveWeapon)//if (rs < 0.9)
	{
		//cout << "Ӣ��������:"<<rs << endl;
		gameInfo.selfMonster[gameInfo.selfMonsterNum].x = 520;
		gameInfo.selfMonster[gameInfo.selfMonsterNum].y = 620;
		gameInfo.selfMonsterNum++;
	}
	cout << "ʶ��Է��������" << endl;
	Mat otherFloor;
	//Rect(204, 229, 630, 118)
	src(otherFloorRect).copyTo(otherFloor);
	recoOtherMonster(otherFloor, gameInfo);
	Sleep(1000);
}

char* ProcessImage::CaptureAnImage(HWND hWnd)
{
	char *lpbitmap = NULL;
	HDC hdcScreen;
	HDC hdcWindow;
	HDC hdcMemDC = NULL;
	HBITMAP hbmScreen = NULL;
	BITMAP bmpScreen;

	// Retrieve the handle to a display device context for the client 
	// area of the window. 
	hdcScreen = GetDC(NULL);
	hdcWindow = GetDC(hWnd);

	// Create a compatible DC which is used in a BitBlt from the window DC
	hdcMemDC = CreateCompatibleDC(hdcWindow);

	if (!hdcMemDC)
	{
		goto done;
	}

	// Get the client area for size calculation
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	POINT rcClientTopLeft;
	rcClientTopLeft.x = rcClient.left;
	rcClientTopLeft.y = rcClient.top;
	POINT rcClientBottomRight;
	rcClientBottomRight.x = rcClient.right;
	rcClientBottomRight.y = rcClient.bottom;
	RECT rcWnd;
	ClientToScreen(hWnd, &rcClientTopLeft);
	ClientToScreen(hWnd, &rcClientBottomRight);
	GetWindowRect(hWnd, &rcWnd);
	rcWnd.left = rcClientTopLeft.x;
	rcWnd.top = rcClientTopLeft.y;
	rcWnd.right = rcClientBottomRight.x;
	rcWnd.bottom = rcClientBottomRight.y;

	//This is the best stretch mode
	SetStretchBltMode(hdcWindow, HALFTONE);
	/*GetSystemMetrics(SM_CXSCREEN),
	GetSystemMetrics(SM_CYSCREEN),*/

	//The source DC is the entire screen and the destination DC is the current window (HWND)
	if (!StretchBlt(hdcWindow,
		0, 0,
		rcClient.right, rcClient.bottom,
		hdcScreen,
		rcWnd.left, rcWnd.top,
		rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top,
		SRCCOPY))
	{
		goto done;
	}

	// Create a compatible bitmap from the Window DC
	hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

	if (!hbmScreen)
	{
		goto done;
	}

	// Select the compatible bitmap into the compatible memory DC.
	SelectObject(hdcMemDC, hbmScreen);

	// Bit block transfer into our compatible memory DC.
	if (!BitBlt(hdcMemDC,
		0, 0,
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		hdcWindow,
		0, 0,
		SRCCOPY))
	{
		goto done;
	}

	// Get the BITMAP from the HBITMAP
	GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

	BITMAPFILEHEADER   bmfHeader;
	BITMAPINFOHEADER   bi;

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bmpScreen.bmWidth;
	bi.biHeight = bmpScreen.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

	// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
	// call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
	// have greater overhead than HeapAlloc.
	HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
	lpbitmap = (char *)GlobalLock(hDIB);

	// Gets the "bits" from the bitmap and copies them into a buffer 
	// which is pointed to by lpbitmap.
	GetDIBits(hdcWindow, hbmScreen, 0,
		(UINT)bmpScreen.bmHeight,
		lpbitmap,
		(BITMAPINFO *)&bi, DIB_RGB_COLORS);

	// A file is created, this is where we will save the screen capture.
	HANDLE hFile = CreateFile("captureqwsx.bmp",
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	// Add the size of the headers to the size of the bitmap to get the total file size
	DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	//Size of the file
	bmfHeader.bfSize = dwSizeofDIB;

	//bfType must always be BM for Bitmaps
	bmfHeader.bfType = 0x4D42; //BM   

	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

	//Unlock and Free the DIB from the heap
	GlobalUnlock(hDIB);
	GlobalFree(hDIB);

	////Close the handle for the file that was created
	CloseHandle(hFile);

	//Clean up
done:
	DeleteObject(hbmScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);
	ReleaseDC(hWnd, hdcWindow);

	return lpbitmap;
}

cv::Mat ProcessImage::getCurrentImage()
{
	HDC hDC = ::GetWindowDC(hWnd);//�õ����
	assert(hDC);

	//HDC hMemDC = ::CreateCompatibleDC(hDC);//��������dc

	//assert(hMemDC);

	RECT rc;
	//	::GetWindowRect(hWnd, &rc);//�˾��ΰ���������߿�
	::GetClientRect(hWnd, &rc);//�õ����沿�־��δ�С
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	//HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);//����λͼ
	//assert(hBitmap);

	//HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, hBitmap);

	//::PrintWindow(hWnd, hMemDC, PW_CLIENTONLY);//�õ�����

	//BITMAPINFOHEADER bi = { 0 };
	//CONST int nBitCount = 24;
	//bi.biSize = sizeof(BITMAPINFOHEADER);
	//bi.biWidth = width;
	//bi.biHeight = height;
	//bi.biPlanes = 1;
	//bi.biBitCount = nBitCount;
	//bi.biCompression = BI_RGB;
	//DWORD dwSize = width * 3 * height;
	//unsigned char *data = new unsigned char[dwSize];
	//::GetDIBits(hMemDC, hBitmap, 0, height, data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);//�����������data��
	char *data = CaptureAnImage(hWnd);
	//Mat image(height, width, CV_8UC3, data);//��ucharת��Ϊmat���ͣ�����opencv
	Mat image = imread("captureqwsx.bmp");
	//flip(image, image, 0);//ͼ�����·�ת
	//imshow("1", image);
	//waitKey(0);

	//::SelectObject(hMemDC, hOldBmp);
	//::DeleteObject(hBitmap);
	//::DeleteObject(hMemDC);
	//::ReleaseDC(hWnd, hDC);

	return image;
}

void ProcessImage::otherTurn(Mat src, GameInfo &gameInfo)
{
	Sleep(1000);
	Mat image;
	src(endButton).copyTo(image);
	int i = 0, j = 0;
	uchar r = image.at<Vec3b>(i, j)[0];
	uchar g = image.at<Vec3b>(i, j)[1];
	uchar b = image.at<Vec3b>(i, j)[2];
	if (abs(r - b) > 100 || abs(g - b) > 100)
	{
		Sleep(5000);
		gameInfo.state = STATE_SELFTURN_PLAY;
		process(gameInfo);
		//Mat src = getCurrentImage();
		//selfTurn(src, gameInfo);
	}
}

float ProcessImage::compareImageBySub(Mat &a, Mat &a2)
{
	int sum = 0;
	for (int i = 0; i < a.rows; i++)//��ֵ������ɫ���£������Ϊ��ɫ
	{
		for (int j = 0; j < a.cols; j++)
		{
			uchar r = a.at<Vec3b>(i, j)[0];
			uchar g = a.at<Vec3b>(i, j)[1];
			uchar b = a.at<Vec3b>(i, j)[2];

			uchar r2 = a2.at<Vec3b>(i, j)[0];
			uchar g2 = a2.at<Vec3b>(i, j)[1];
			uchar b2 = a2.at<Vec3b>(i, j)[2];
			if (abs(r - r2) < 20 && abs(b2 - b) < 20 && abs(g - g2) < 20)
			{
				sum++;
			}
		}
	}
	return (float)sum / (a.rows*a.cols);
}

int ProcessImage::compareImage(Mat &a, Mat &b)
{
	auto orb = ORB::create();
	vector<KeyPoint> keyPoints_1, keyPoints_2;
	Mat descriptors_1, descriptors_2;
	orb->detectAndCompute(a, Mat(), keyPoints_1, descriptors_1);
	orb->detectAndCompute(b, Mat(), keyPoints_2, descriptors_2);
	//	cout << clock() - t_start << endl;
	//2.BruteForceMatcherƥ��
	//BruteForceMatcher<HammingLUT> matcher;  //Ҳ����ʹ��ruteForce<Hamming>
	auto matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
	vector<DMatch> matches;
	matcher->match(descriptors_1, descriptors_2, matches);
	//	cout << matches.size() << endl;
	//	cout << clock() - t_start << endl;
	//3.����ƥ���
	double max_dist = 0; double min_dist = 100;
	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_1.rows; i++)
	{
		double dist = matches[i].distance;
		//cout << ":"<<dist << endl;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	//	printf("-- Max dist : %f \n", max_dist);
	//	printf("-- Min dist : %f \n", min_dist);
	//	cout << clock() - t_start << endl;
	//-- Draw only "good" matches (i.e. whose distance is less than 0.6*max_dist )
	//-- PS.- radiusMatch can also be used here.
	std::vector< DMatch > good_matches;
	for (int i = 0; i < descriptors_1.rows; i++)
	{
		if (matches[i].distance <= 0.6*max_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}
	return (int)good_matches.size();
}

int ProcessImage::compareImageByORB(Mat img_1)
{
	clock_t t_start = clock();
	Mat img_2;
	int maxi = 1, maxn = 0;
	for (int i = 0; i < 30; ++i)
	{
		//cout << "��" << i << "��ͼƬ" << endl;
		stringstream ss;
		ss << i;
		string s = ss.str();
		img_2 = imread(CARDSINFO + s + ".png");
		if (!img_1.data || !img_2.data)
		{
			//		cout << "error reading images " << endl;
			break;
		}
		//	cout << clock() - t_start << endl;
			//1.orb��������㲢��ȡ����ֵ
		auto orb = ORB::create();
		vector<KeyPoint> keyPoints_1, keyPoints_2;
		Mat descriptors_1, descriptors_2;
		orb->detectAndCompute(img_1, Mat(), keyPoints_1, descriptors_1);
		orb->detectAndCompute(img_2, Mat(), keyPoints_2, descriptors_2);
		//	cout << clock() - t_start << endl;
			//2.BruteForceMatcherƥ��
			//BruteForceMatcher<HammingLUT> matcher;  //Ҳ����ʹ��ruteForce<Hamming>
		auto matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
		vector<DMatch> matches;
		matcher->match(descriptors_1, descriptors_2, matches);
		//	cout << matches.size() << endl;
		//	cout << clock() - t_start << endl;
			//3.����ƥ���
		double max_dist = 0; double min_dist = 100;
		//-- Quick calculation of max and min distances between keypoints
		for (int i = 0; i < descriptors_1.rows; i++)
		{
			double dist = matches[i].distance;
			if (dist < min_dist) min_dist = dist;
			if (dist > max_dist) max_dist = dist;
		}
		//	printf("-- Max dist : %f \n", max_dist);
		//	printf("-- Min dist : %f \n", min_dist);
		//	cout << clock() - t_start << endl;
			//-- Draw only "good" matches (i.e. whose distance is less than 0.6*max_dist )
			//-- PS.- radiusMatch can also be used here.
		std::vector< DMatch > good_matches;
		for (int i = 0; i < descriptors_1.rows; i++)
		{
			if (matches[i].distance < 0.6*max_dist)
			{
				good_matches.push_back(matches[i]);
			}
		}
		//	cout << "goodMatches:" << good_matches.size() << endl;
		//	cout << clock() - t_start << endl;
		if (good_matches.size() > maxn)
		{
			maxn = good_matches.size();
			maxi = i;
		}
	}
	/*imshow("ԭͼ��", img_1);
	stringstream ss;
	ss << maxi;
	string s = ss.str();
	img_2 = imread(CARDSINFO + s + ".png");
	imshow("�Ƚ���ӽ�ͼ��", img_2);
	waitKey(0);*/
	return maxi;
}


