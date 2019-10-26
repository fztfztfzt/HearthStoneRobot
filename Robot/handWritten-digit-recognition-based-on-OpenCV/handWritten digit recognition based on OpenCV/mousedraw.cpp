#include "mousedraw.h"

BoxExtractor::BoxExtractor()
{
}

void BoxExtractor::mouseHandler(int event, int x, int y, int flags, void *param) 
{
	BoxExtractor *self = static_cast<BoxExtractor*>(param);
	self->opencv_mouse_callback(event, x, y, flags, param);
}

void BoxExtractor::opencv_mouse_callback(int event, int x, int y, int, void *param) 
{
	handlerT * data = (handlerT*)param;
	switch (event) 
	{
		// update the selected bounding box
	case EVENT_MOUSEMOVE:
		if (data->isDrawing) 
		{
			data->points.push_back(cv::Point(x, y));
		}
		break;
		// start to select the bounding box
	case EVENT_LBUTTONDOWN:
		data->isDrawing = true;
		data->points.clear();
		data->points.push_back(cv::Point(x, y));
		break;
		// cleaning up the selected bounding box
	case EVENT_LBUTTONUP:
		data->isDrawing = false;
		break;
	}
}

Mat BoxExtractor::MouseDraw(Mat& img)
{
	MouseDraw("DRAW AN DIGIT", img, Scalar(0, 0, 0), 4);
	return img;
}

int BoxExtractor::MouseDraw(const std::string& windowName, Mat& img, Scalar color, int border) 
{
	int key = 0;
	imshow(windowName, img);
	printf("利用鼠标写下一个或多个数字，按回车输出预测结果并进行下一次书写，esc退出\n");
	params.image = img.clone();
	setMouseCallback(windowName, mouseHandler, (void *)&params);
	int cnt = 0;
	while (!(key == 32 || key == 27 || key == 13)) 
	{
		int length = params.points.size();
		for (int i = 0; i<length && length > 2; i++) 
		{
			if (i == length - 1) 
			{
				cv::line(params.image, params.points.at(i), params.points.at(i), color, border);
			}
			else 
			{
				cv::line(params.image, params.points.at(i), params.points.at(i + 1), color, border);
			}
		}
		imshow(windowName, params.image);
		//        params.image=img.clone();
		key = waitKey(1);
	}
	params.image.copyTo(img);
	return key;
}