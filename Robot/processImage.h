/*
使用opencv库对图像进行处理
*/
#pragma once
#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>
#include "gameInfo.h"
using namespace cv;
using namespace std;
int num[11][9] = { { 0, 1, 1, 1, 0, 1, 1, 1, 1 }, { 0, 1, 1, 0, 1, 1, 0, 1, 1 }, { 1, 0, 1, 1, 0, 1, 0, 1, 0 }, { 0, 1, 0, 0, 1, 1, 0, 0, 1 },
{ 0, 0, 1, 0, 0, 1, 0, 1, 1 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 1, 0, 1, 1, 1, 1 }, { 1, 1, 1, 0, 1, 0, 0, 1, 0 },
{ 0, 0, 1, 0, 1, 1, 1, 0, 0 }, { 1, 1, 1, 1, 0, 1, 0, 0, 0 }, { 0, 1, 1, 1, 0, 1, 1, 1, 0 } };
vector<vector<vector<bool>>> numModel;
class ProcessImage
{
public:
	ProcessImage(){
		numModel.resize(11);//读取数字模型
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
	}
	void process(Mat src, GameInfo &gameInfo)
	{
		switch (gameInfo.state)
		{
		case STATE_CHANGECARDSTART:
			changeCardStart(src, gameInfo);
			break;
		case STATE_SELFTURN:
			selfTrun(src, gameInfo);
			break;
		case STATE_OTHERTURN:
			otherTrun(src, gameInfo);
			break;
		default:
			break;
		}
	}
	void changeCardStart(Mat src, GameInfo &gameInfo)//预处理发牌阶段图像
	{
		for (int i = 0; i<src.rows; i++)//阈值处理，白色留下，其余改为黑色
		{
			for (int j = 0; j<src.cols; j++)
			{
				uchar r = src.at<Vec3b>(i, j)[0];
				uchar g = src.at<Vec3b>(i, j)[1];
				uchar b = src.at<Vec3b>(i, j)[2];
				if (r < 30 && g >= 70 &&g<=140 && b < 30)
				{
					src.at<Vec3b>(i, j)[0] = 0;
					src.at<Vec3b>(i, j)[1] = 0;
					src.at<Vec3b>(i, j)[2] = 0;
				}
				
			}
		}
		Mat gray;
		cvtColor(src, gray, CV_BGR2GRAY);
		//imshow("灰度",gray);
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
		findContours(thr,			//图像
			contours,				//轮廓点
			//包含图像拓扑结构的信息（可选参数，这里没有选）
			CV_RETR_EXTERNAL,			//获取轮廓的方法（这里获取外围轮廓）
			CV_CHAIN_APPROX_NONE);		//轮廓近似的方法（这里不近似，获取全部轮廓）
		//打印轮廓信息
		std::cout << "共有外围轮廓：" << contours.size() << "条" << std::endl;
		std::vector<std::vector<Point>>::const_iterator itContours = contours.begin();
		for (; itContours != contours.end(); ++itContours)
		{
			std::cout << "每个轮廓的长度: " << itContours->size() << std::endl;
		}




		Mat result(thr.size(), CV_8U, Scalar(255));
		result.setTo(Scalar(255));
		drawContours(result, contours, -1, Scalar(0), 2);
		//imshow("提取外围轮廓", result);

		//除去太长或者太短的轮廓
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

		//画出轮廓

		//画出轮廓，参数为：画板，轮廓，轮廓指示（这里画出所有轮廓），颜色，线粗
		//	drawContours(result, contours, -1, Scalar(0), 2);
		//将轮廓重绘于白板上
		result.setTo(Scalar(255));
		drawContours(result, contours, -1, Scalar(0), 1);
		//imshow("去除较小的", result); //还要注意提取轮廓的方法还有很多种，比如CV_RETR_LIST代表所有轮廓
		//waitKey(0);
		//轮廓的形状描述子
		//外接矩形
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
			gameInfo.handCard[i].spend = getNumByFullCard(matio[i]);
			gameInfo.handCard[i].x = r0.x + r0.width / 2;
			gameInfo.handCard[i].y = r0.y + r0.height / 2;
		}
	//	imshow("外接矩形", src);
	}
	int getNumByImage(Mat src)
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
		findContours(cz,			//图像
			contours,				//轮廓点
			//包含图像拓扑结构的信息（可选参数，这里没有选）
			CV_RETR_EXTERNAL,			//获取轮廓的方法（这里获取外围轮廓）
			CV_CHAIN_APPROX_NONE);		//轮廓近似的方法（这里不近似，获取全部轮廓）
		//打印轮廓信息
		std::cout << "共有外围轮廓：" << contours.size() << "条" << std::endl;
		for (int i = 0; i < contours.size(); ++i)
		{
			cout << contours[i].size() << endl;;
		}
		Rect r0 = boundingRect(Mat(contours[contours.size() - 1]));
		Mat matio;
		thr(r0).copyTo(matio);
		rectangle(src, r0, Scalar(0), 2);
		//	imshow("222", matio);
		//	waitKey(0);
		return recoNum(matio);
	}
	int getNumByFullCard(Mat src)//提取数字部分轮廓
	{
		Mat image;
		src(Rect(0, 0, src.rows / 4, src.cols / 3)).copyTo(image);
		return getNumByImage(image);		
	}
	int recoNum(Mat src)//数字识别
	{
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
			//cout << num << " ";
			if (num>w*h / 2) temp[i] = 1, cout << "1" << ",";
			else cout << "0" << ",";
			//if (i % 3 == 2) cout << endl;
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
				if (sum>maxsum)
				{
					maxsum = sum;
					maxn = i;
				}
			}
		}
		cout << "识别出的数字是：" << maxn << endl;
		return maxn;
	}

	void selfTrun(Mat src, GameInfo &gameInfo)//处理出牌阶段场景
	{
		Mat image,src2;
		src(Rect(260,660,440,src.rows-660)).copyTo(src2);
		src2.copyTo(image);
		for (int i = 0; i<src2.rows; i++)//阈值处理，白色留下，其余改为黑色
		{
			for (int j = 0; j<src2.cols; j++)
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
		cvtColor(image, gray, CV_BGR2GRAY);//转换为单通道便于进行轮廓提取
		//imshow("gray", gray);

		//Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));//腐蚀膨胀效果不好
		//dilate(thr, thr, element);
		//erode(thr, thr, element);
		//dilate(thr, thr, element);
		//erode(thr, thr, element);
		//imshow("fushi", thr);
		vector<std::vector<Point>> contours;
		findContours(gray,			//图像
			contours,				//轮廓点
			//包含图像拓扑结构的信息（可选参数，这里没有选）
			CV_RETR_EXTERNAL,			//获取轮廓的方法（这里获取外围轮廓）
			CV_CHAIN_APPROX_NONE);		//轮廓近似的方法（这里不近似，获取全部轮廓）
		//打印轮廓信息
		std::cout << "共有外围轮廓：" << contours.size() << "条" << std::endl;
		std::vector<std::vector<Point>>::const_iterator itContours = contours.begin();
		for (; itContours != contours.end(); ++itContours)
		{
			std::cout << "每个轮廓的长度: " << itContours->size() << std::endl;
		}




	/*	Mat result(gray.size(), CV_8U, Scalar(255));
		result.setTo(Scalar(255));
		drawContours(result, contours, -1, Scalar(0), 2);
		imshow("提取外围轮廓", result);*/

		//除去太长或者太短的轮廓
		int cmin = 10;
		int cmax = 100;
		std::vector<std::vector<Point>>::const_iterator itc = contours.begin();
		while (itc != contours.end())
		{
			if (itc->size() < cmin || itc->size() > cmax)
				itc = contours.erase(itc);
			else
				++itc;
		}

		//画出轮廓，参数为：画板，轮廓，轮廓指示（这里画出所有轮廓），颜色，线粗
		//	drawContours(result, contours, -1, Scalar(0), 2);
		//将轮廓重绘于白板上
		//result.setTo(Scalar(255));
		//drawContours(result, contours, -1, Scalar(0), 1);
		//imshow("去除较小的", result); //还要注意提取轮廓的方法还有很多种，比如CV_RETR_LIST代表所有轮廓
		//waitKey(0);
		//轮廓的形状描述子
		//外接矩形
		Mat matio[100];
		gameInfo.currentNum = 0;
		for (int i = 0; i < contours.size(); ++i)
		{
			Rect r0 = boundingRect(Mat(contours[i]));
			if (r0.width >= r0.height) continue;
			src2(r0).copyTo(matio[i]);
			//rectangle(src2, r0, Scalar(0), 2);
			imshow("iiiiiiiii", matio[i]);
			
			gameInfo.handCard[gameInfo.currentNum].spend = getNumByImage(matio[i]);
			gameInfo.handCard[gameInfo.currentNum].x = 260+r0.x + r0.width / 2+10;
			gameInfo.handCard[gameInfo.currentNum].y = 660+r0.y + r0.height / 2+30;
			cout << "识别出的卡牌信息：费用：" << gameInfo.handCard[gameInfo.currentNum].spend << "位置:" << gameInfo.handCard[gameInfo.currentNum].x << " " << gameInfo.handCard[gameInfo.currentNum].y<< endl;
			gameInfo.currentNum++;
			waitKey(0);
		}
		//imshow("外接矩形", src);

		//	findContours(thr,			//图像 
		//	contours,				//轮廓点
		//	//包含图像拓扑结构的信息（可选参数，这里没有选）
		//	CV_RETR_LIST,			//获取轮廓的方法（这里获取所有轮廓）
		//	CV_CHAIN_APPROX_NONE);		//轮廓近似的方法（这里不近似，获取全部轮廓
		////画出轮廓
		//drawContours(result, contours, -1, Scalar(0), 2);
		//imshow("提取所有轮廓", result);
	}
	void otherTrun(Mat src, GameInfo &gameInfo)
	{
		Mat image;
		src(Rect(924, 340, 45, 10)).copyTo(image);
		int i = 0, j = 0;
		uchar r = image.at<Vec3b>(i, j)[0];
		uchar g = image.at<Vec3b>(i, j)[1];
		uchar b = image.at<Vec3b>(i, j)[2];
		if (abs(r - b) > 100 || abs(g - b) > 100)
		{
			gameInfo.state = STATE_SELFTURN;
			selfTrun(src, gameInfo);
		}
	}
private:
	const int MAXV = 1;
	
};