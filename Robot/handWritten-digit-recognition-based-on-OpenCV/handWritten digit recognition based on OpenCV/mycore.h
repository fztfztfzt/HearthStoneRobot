#pragma once

#ifndef MYCORE_H
#define MYCORE_H

#include<opencv2\opencv.hpp>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

using namespace cv;
using namespace cv::ml;
using namespace std;

#include "mousedraw.h"
#include "global.h"
#include "loadMNIST.h"
#include"SVM_on_handWritten.h"
#include "segment.h"
#include"features.h"
#include"preprocess.h"
#include"recognize.h"

#endif