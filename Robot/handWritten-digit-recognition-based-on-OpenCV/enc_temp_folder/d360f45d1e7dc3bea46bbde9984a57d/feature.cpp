#include "opencv2/imgproc/types_c.h"
#include "features.h"

//调整图像大小
void resizeImage(Mat& srcImage, Size size)
{
	if (srcImage.rows * srcImage.cols < size.area())
		resize(srcImage, srcImage, size, 0, 0, INTER_CUBIC);
	else if (srcImage.rows * srcImage.cols > size.area())
		resize(srcImage, srcImage, size, 0, 0, INTER_AREA);
}

//图像 二值初始化
void binaryInit(Mat& srcDataMat, Size size)
{
	srcDataMat.convertTo(srcDataMat, CV_8UC1);
	normal(srcDataMat, srcDataMat);
	resizeImage(srcDataMat, size);
	threshold(srcDataMat, srcDataMat, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);//阈值化要求图像为CV_8UC1
	srcDataMat.convertTo(srcDataMat, CV_32FC1);//机器学习算法中输入向量为CV_32FC1
}

//图像归一化
void normal(Mat& srcImage, Mat& dstImage)
{
	//找到有效区域roi，及字符的外接矩形
	
	int bottom = srcImage.rows + 1;
	int top = 0;
	int left = srcImage.cols + 1;
	int right = 0;
	for (int i = 0; i < srcImage.rows; ++i)
	{
		for (int j = 0; j < srcImage.cols; ++j)
		{
			if (srcImage.at<uchar>(i, j) > 0)
			{
				bottom = min(bottom, i);
				top = max(top, i);
				left = min(left, j);
				right = max(right, j);
			}
		}
	}
	Rect rec = Rect(left, bottom, right - left + 1, top - bottom + 1);
	Mat roi = srcImage(rec);

	//图片居中并归一化
	int width = roi.cols;
	int height = roi.rows;
	int longLen = max(width, height);
	int shortLen = width + height - longLen;

	dstImage = Mat(longLen, longLen, CV_8UC1, Scalar::all(0));
	roi.copyTo(dstImage(Rect((longLen - width) / 2, (longLen - height) / 2, width, height)));
	imshow("shit2", dstImage);
	waitKey(1000);
}




void image2Vec(const Mat& srcImage, Mat& dstImage)
{
	if (srcImage.isContinuous())
	{
		dstImage = srcImage.reshape(1, 1);
	}
	else
	{
		for (int i = 0; i < srcImage.rows; ++i)
		{
			for (int j = 0; j < srcImage.cols; ++i)
			{
				dstImage.at<float>(i * srcImage.cols + j) = srcImage.at<float>(i, j);
			}
		}
	}
}

//统计srcDataMat中大于阈值的像素个数
float calBigNumber(const Mat& srcDataMat, float thresh)
{
	float sum = 0.0;
	for (int i = 0; i < srcDataMat.rows; ++i)
	{
		for (int j = 0; j < srcDataMat.cols; ++j)
		{
			if (srcDataMat.at<float>(i, j) > thresh)
			{
				sum += 1.0;
			}
		}
	}
	return sum;
}

//计算直方图特征
void getHistogram(const Mat& srcMat, Mat& histogramMat, int flag)//flag为0时计算行的直方图，1计算列
{
	float sum = 0.0;
	int size = flag == 0? srcMat.rows : srcMat.cols;
	Mat listOfMat;
	for (int i = 0; i < size; ++i)
	{
		listOfMat = flag == 0 ? srcMat.row(i) : srcMat.col(i);
		sum = calBigNumber(listOfMat, 0);
		histogramMat.at<float>(0, i) = sum;
	}
	normalize(histogramMat, histogramMat, 1.0, 0.0, NORM_MINMAX);
}

//8 * 8全部像素和28 * 28直方图混合特征
void getMixedFeature(const Mat& srcDataMat, Mat& dstDataMat, Size size)
{
	Mat tmpDataMat1;
	getAllPixelsFeature(srcDataMat, tmpDataMat1, Size(8, 8));
	normalize(tmpDataMat1, tmpDataMat1, 1.0, 0.0, NORM_MINMAX);//归一化为0，1区间，方便调参

	Mat tmpDataMat2;
	getHistogramFeature(srcDataMat, tmpDataMat2, size);

	Mat tmpDataMat3 = Mat::zeros(1, tmpDataMat1.cols + tmpDataMat2.cols, CV_32FC1);
	int cnt = 0;
	for (int i = 0; i < tmpDataMat1.cols; ++i)
	{
		tmpDataMat3.at<float>(0, cnt++) = tmpDataMat1.at<float>(i);
	}
	for (int i = 0; i < tmpDataMat2.cols; ++i)
	{
		tmpDataMat3.at<float>(0, cnt++) = tmpDataMat2.at<float>(i);
	}
	dstDataMat = tmpDataMat3.clone();
}

//全部像素特征
void getAllPixelsFeature(const Mat& srcDataMat, Mat& dstDataMat, Size size)
{
	Mat tmpDataMat = srcDataMat.clone();
	binaryInit(tmpDataMat, size);
	image2Vec(tmpDataMat, dstDataMat);
}

//直方图特征
void getHistogramFeature(const Mat& srcDataMat, Mat& dstDataMat, Size size)
{
	Mat tmpDataMat = srcDataMat.clone();
	binaryInit(tmpDataMat, size);

	Mat rowHistogram = Mat::zeros(1, tmpDataMat.rows, CV_32FC1), colHistogram = Mat::zeros(1, tmpDataMat.cols, CV_32FC1);
	getHistogram(tmpDataMat, rowHistogram, 0);
	getHistogram(tmpDataMat, colHistogram, 1);

	int cnt = 0;
	Mat tmpDstMat = Mat::zeros(1, rowHistogram.cols + colHistogram.cols, CV_32FC1);
	for (int i = 0; i < rowHistogram.cols; ++i)
	{
		tmpDstMat.at<float>(0, cnt++) = rowHistogram.at<float>(0, i);
	}
	for (int i = 0; i < colHistogram.cols; ++i)
	{
		tmpDstMat.at<float>(0, cnt++) = colHistogram.at<float>(0, i);
	}
	dstDataMat = tmpDstMat.clone();
}