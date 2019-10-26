#pragma once

#ifndef BOXEXTRACTOR_H
#define BOXEXTRACTOR_H

#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
using namespace std;
using namespace cv;

class BoxExtractor 
{
public:
	BoxExtractor();
	Mat MouseDraw(Mat& img);
	int MouseDraw(const std::string& windowName, Mat& img, Scalar color, int border);

	struct handlerT 
	{
		bool isDrawing;
		std::vector<cv::Point> points;
		Mat image;
		handlerT() : isDrawing(false) {};
	}params;

private:
	static void mouseHandler(int event, int x, int y, int flags, void *param);
	void opencv_mouse_callback(int event, int x, int y, int, void *param);
};

#endif // BOXEXTRACTOR_H