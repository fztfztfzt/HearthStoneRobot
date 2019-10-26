#pragma once

#ifndef LOAD_MNIST_H
#define LOAD_MNIST_H

#include<vector>
#include<string>
#include<opencv2\opencv.hpp>

using namespace cv;
using namespace std;

//MNIST���ݿ����
int ReverseInt(int i);

void ReadMNIST(int NumberOfImages, const string& fileName, vector<Mat>& arr);

void ReadMNISTLabel(int NumberOfImages, const string& fileName, vector<int>& arr);

//����MNIST����
void loadMNISTData(vector<Mat>& trainImageVec, vector<int>& trainLabelVec, int trainNumber,
	vector<Mat>& testImageVec, vector<int>& testLabelVec, int testNumber);

//�������ݲ��Ժ���
void testFileLoad(const vector<Mat>& trainImage, const vector<int>& trainLabel);

#endif