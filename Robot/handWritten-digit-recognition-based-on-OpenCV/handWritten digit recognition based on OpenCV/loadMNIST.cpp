#include<iostream>
#include<fstream>
#include<vector>
#include<opencv2\opencv.hpp>

#include"global.h"
using namespace std;
using namespace cv;

int ReverseInt(int i)
{
	unsigned char ch1, ch2, ch3, ch4;
	ch1 = i & 255;
	ch2 = (i >> 8) & 255;
	ch3 = (i >> 16) & 255;
	ch4 = (i >> 24) & 255;
	return((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}

void ReadMNIST(int NumberOfImages, const string& fileName, vector<Mat>& arr)
{
	ifstream file(fileName, ios::binary);
	if (file.is_open())
	{
		int magic_number = 0;
		int number_of_images = 0;
		int n_rows = 0;
		int n_cols = 0;
		file.read((char*)&magic_number, sizeof(magic_number));
		magic_number = ReverseInt(magic_number);
		file.read((char*)&number_of_images, sizeof(number_of_images));
		number_of_images = ReverseInt(number_of_images);
		file.read((char*)&n_rows, sizeof(n_rows));
		n_rows = ReverseInt(n_rows);
		file.read((char*)&n_cols, sizeof(n_cols));
		n_cols = ReverseInt(n_cols);
		for (int i = 0; i<NumberOfImages; ++i)
		{
			Mat tmpMat(n_rows, n_cols, CV_8UC1);
			for (int r = 0; r<n_rows; ++r)
			{
				file.read((char*)tmpMat.ptr<uchar>(r), sizeof(uchar) * n_cols);
			}
			arr.push_back(tmpMat);
		}
	}
}

void ReadMNISTLabel(int NumberOfImages, const string& fileName, vector<int>& arr)
{
	ifstream file(fileName, ios::binary);
	if (file.is_open())
	{
		int magicNumber = 0;
		file.read((char*)& magicNumber, sizeof(magicNumber));
		magicNumber = ReverseInt(magicNumber);
		int numberOfLabels = 0;
		file.read((char*)& numberOfLabels, sizeof(numberOfLabels));
		numberOfLabels = ReverseInt(numberOfLabels);
		for (int i = 0; i < NumberOfImages; ++i)
		{
			char tmpName;
			file.read((char*)& tmpName, sizeof(tmpName));
			arr.push_back((int)tmpName);
		}
	}
}

void testFileLoad(const vector<Mat>& trainImage, const vector<int>& trainLabel)
{
	for (int i = 0; i < 10; ++i)
	{
		cout << trainLabel[i] << endl;
	}
	for (int i = 0; i < 10; ++i)
	{
		Mat image = trainImage[i];
		imshow("fuck", image);
		waitKey(1000);
	}
}

void loadMNISTData(vector<Mat>& trainImageVec, vector<int>& trainLabelVec, int trainNumber,
	vector<Mat>& testImageVec, vector<int>& testLabelVec, int testNumber)
{
	ReadMNIST(trainNumber, g_TrainImageFileName, trainImageVec);
	ReadMNISTLabel(trainNumber, g_TrainLabelFileName, trainLabelVec);

	ReadMNIST(testNumber, g_TestImageFileName, testImageVec);
	ReadMNISTLabel(testNumber, g_TestLabelFileName, testLabelVec);
}
