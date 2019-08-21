#pragma once

#ifndef LOAD_MNIST_H
#define LOAD_MNIST_H

#include<vector>
#include<string>
#include<opencv2\opencv.hpp>

using namespace cv;
using namespace std;

//MNIST数据库读入
int ReverseInt(int i);

void ReadMNIST(int NumberOfImages, const string& fileName, vector<Mat>& arr);

void ReadMNISTLabel(int NumberOfImages, const string& fileName, vector<int>& arr);

//读入MNIST数据
void loadMNISTData(vector<Mat>& trainImageVec, vector<int>& trainLabelVec, int trainNumber,
	vector<Mat>& testImageVec, vector<int>& testLabelVec, int testNumber);

//读入数据测试函数
void testFileLoad(const vector<Mat>& trainImage, const vector<int>& trainLabel);

#endif