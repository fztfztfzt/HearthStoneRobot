#pragma once

#ifndef GLOBLE_H
#define GLOBLE_H

#include<string>
using namespace std;

#define WINDOW_NAME "��д����ʶ��"

//MNIST���ݿ��ļ�
const string g_TestImageFileName = "t10k-images.idx3-ubyte";
const string g_TestLabelFileName = "t10k-labels.idx1-ubyte";
const string g_TrainImageFileName = "train-images.idx3-ubyte";
const string g_TrainLabelFileName = "train-labels.idx1-ubyte";

//SVM��xml�ļ�
const string g_SVM = "SVM.xml";//����ѵ���õ�SVM
const string g_SVMtemp = "SVM2.xml";//��ʱѵ���ļ�
const string g_SVMtemp2 = "SVM3.xml";//��ʱѵ���ļ�

#endif