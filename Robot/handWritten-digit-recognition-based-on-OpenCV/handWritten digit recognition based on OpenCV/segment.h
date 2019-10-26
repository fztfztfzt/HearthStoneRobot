#pragma once

#ifndef SEGMENT_H
#define SEGMENT_H
#include "opencv2/imgproc/types_c.h"
#include<opencv2\opencv.hpp>
#include<iostream>
#include<string>

using namespace cv;
using namespace std;

#include "mousedraw.h"
#include "global.h"
#include"features.h"

void getSegment(const Mat& srcImage, vector<Mat>& arr, Mat& showImage);

bool cmp(const Rect& a, const Rect& b);

bool cmp2(const Rect& a, const Rect& b);

#endif