#pragma once

#ifndef FEATURES_H

#define FEATURES_H

#include<opencv2\opencv.hpp>
#include<vector>
#include<string>
#include<iostream>
#include<fstream>

using namespace std;
using namespace cv;

typedef void(*getSVMfeature)(const Mat& srcDataMat, Mat& dstDataMat, Size size);

void resizeImage(Mat& srcImage, Size size);

void binaryInit(Mat& srcDataMat, Size size);

void getHistogramFeature(const Mat& srcDataMat, Mat& dstDataMat, Size size);

void getAllPixelsFeature(const Mat& srcDataMat, Mat& dstDataMat, Size size);

void getMixedFeature(const Mat& srcDataMat, Mat& dstDataMat, Size size);

void image2Vec(const Mat& srcImage, Mat& dstImage);

void normal(Mat& srcImage, Mat& dstImage);


#endif