#pragma once

#ifndef SVM_ON_HAND_H
#define SVM_ON_HAND_H

#include<opencv2\opencv.hpp>
#include<string>
#include<vector>
using namespace std;
using namespace cv;
using namespace cv::ml;

#include"features.h"
#include"loadMNIST.h"
#include"global.h"

class mySVM
{
private:
	vector<Mat> trainImageVec, testImageVec;
	vector<int> trainLabelVec, testLabelVec;
	vector<Mat> trainTestImageVec;
	vector<int> trainTestLabelVec;
	Mat trainDataMat, trainLabelMat;
	Mat testDataMat, testLabelMat;
	Mat trainTestDataMat, trainTestLabelMat;

	Ptr<SVM> svm;

	void loadSVMData(vector<Mat>& srcImageVec, Mat& dstDataMat, getSVMfeature fun, Size size);

	void loadSVMLabel(const vector<int>& srclabelVec, Mat& dstLabelMat);

public:
	mySVM()
	{
		svm = SVM::create();
	}
	
	mySVM(int trainNumber, int testNumber, int trainTestNumber, getSVMfeature fun, Size size);

	void showSVMInfo();

	void trainSVM(const string& fileName); 

	void getSVM(const string& fileName);

	float testSVM(Mat& testingDataMat, vector<int>& testingLabelVec);

	void testSVM(const string& fileName);

	void trainTestSVM(const string& fileName);
};



#endif