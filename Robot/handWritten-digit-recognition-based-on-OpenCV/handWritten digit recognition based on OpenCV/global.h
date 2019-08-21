#pragma once

#ifndef GLOBLE_H
#define GLOBLE_H

#include<string>
using namespace std;

#define WINDOW_NAME "手写数字识别"

//MNIST数据库文件
const string g_TestImageFileName = "t10k-images.idx3-ubyte";
const string g_TestLabelFileName = "t10k-labels.idx1-ubyte";
const string g_TrainImageFileName = "train-images.idx3-ubyte";
const string g_TrainLabelFileName = "train-labels.idx1-ubyte";

//SVM的xml文件
const string g_SVM = "SVM.xml";//最终训练好的SVM
const string g_SVMtemp = "SVM2.xml";//临时训练文件
const string g_SVMtemp2 = "SVM3.xml";//临时训练文件

#endif