#include "opencv2/imgproc/types_c.h"
#include"preprocess.h"

void preProcess(const Mat& srcImage, Mat& dstImage)
{
	Mat tmpImage = srcImage.clone();
	if (tmpImage.type() != CV_8UC1)
	{
		cvtColor(tmpImage, tmpImage, CV_BGR2GRAY);
	}
	//中值滤波预处理噪点
	//GaussianBlur(tmpImage, tmpImage, Size(3, 3), 0, 0, BORDER_DEFAULT);
	medianBlur(tmpImage, tmpImage, 3);
	//形态学滤波 开运算预处理
	Mat element = getStructuringElement(MORPH_RECT, Size(7,7));
	//morphologyEx(tmpImage, tmpImage, MORPH_OPEN, element);

	//imshow("高斯滤波加开运算", tmpImage);
	//waitKey(500);
	
	//canny提取边沿
	Canny(tmpImage, dstImage, 75, 100, 3);
	//imshow("canny边沿提取", dstImage);
	//waitKey(500);

	//形态学滤波，闭运算终处理
	element = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(dstImage, dstImage, MORPH_DILATE, element);
	//imshow("形态学处理", tmpImage4);
	//waitKey(500);

	//中值滤波，处理噪点
	//medianBlur(tmpImage4, dstImage, 3);
	//imshow("中值滤波后", dstImage);
	//waitKey(500);
}