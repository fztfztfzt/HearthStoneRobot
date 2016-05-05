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
class processImage
{
public:
	processImage(){
		numModel.resize(11);
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
	void process(Mat src,cardInfo &cinfo)//预处理发牌阶段图像
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
		imshow("灰度",gray);
		Mat thr;
		threshold(gray, thr, 50, 255, CV_THRESH_BINARY);
		imshow("gray", thr);
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		dilate(thr, thr, element);
		erode(thr, thr, element);
		//erode(thr, thr, element);
		//dilate(thr, thr, element);
		imshow("fushi", thr);
		waitKey(0);
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
		imshow("提取外围轮廓", result);

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
		imshow("去除较小的", result); //还要注意提取轮廓的方法还有很多种，比如CV_RETR_LIST代表所有轮廓
		waitKey(0);
		//轮廓的形状描述子
		//外接矩形
		Mat matio[10];
		int num = contours.size();
		cinfo.currentNum = num;
		for (int i = 0; i < contours.size(); ++i)
		{
			Rect r0 = boundingRect(Mat(contours[i]));
			src(r0).copyTo(matio[i]);
			rectangle(src, r0, Scalar(0), 2);
			imshow("" + i, matio[i]);
			waitKey(0);
			cinfo.handCard[i] = find2(matio[i]);
		}
		imshow("外接矩形", src);
	}
	int find2(Mat src)//提取数字部分轮廓
	{
		Mat image;
		src(Rect(0, 0, src.rows / 4, src.cols / 3)).copyTo(image);
		imshow("windows", image);
		Mat gray;
		cvtColor(image, gray, CV_BGR2GRAY);
		Mat thr;
		threshold(gray, thr, 250, 255, CV_THRESH_BINARY);
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat cz;
		dilate(thr, cz, element);
		erode(cz, cz, element);
		imshow("gray2", cz);
		waitKey(0);
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
		Rect r0 = boundingRect(Mat(contours[contours.size()-1]));
		Mat matio;
		thr(r0).copyTo(matio);
		rectangle(src, r0, Scalar(0), 2);
		imshow("222", matio);
		waitKey(0);
		return recoNum(matio);
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
private:
	const int MAXV = 1;
	
};