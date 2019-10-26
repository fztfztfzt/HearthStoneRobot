#include"recognize.h"

//基于控制台的手写数字识别
void handWritingRecognize()
{
	Ptr<SVM> svm = Algorithm::load<SVM>(g_SVM);

	Mat srcImage(500, 500, CV_8UC1, Scalar::all(0)), image;
	namedWindow(WINDOW_NAME);

	for (;;)
	{
		BoxExtractor box;
		srcImage.copyTo(image);
		int key = box.MouseDraw(WINDOW_NAME, image, Scalar::all(255), 5);
		if (key != 13)
			break;

		vector<Mat> arr;
		getSegment(image, arr, image);

		Mat imageDataMat;
		cout << "预测结果：" << endl;
		float ans = -1;
		for (int i = 0; i < arr.size(); ++i)
		{
			Mat imageDataMat;
			getMixedFeature(arr[i], imageDataMat, Size(28, 28));
			float ans = (float)svm->predict(imageDataMat);
			cout << ans << " ";
		}
		cout << endl;
		imshow(WINDOW_NAME, image);

		cout << "输入任意字符进行下一次的输入，输入esc回到上一级" << endl;
		key = waitKey(0);
		if (key == 27)
		{
			break;
		}
		else
		{
			continue;
		}
		cout << endl;
	}
	destroyWindow(WINDOW_NAME);
}

int handWritingImageRecognize(Mat imageSrc)
{
	Ptr<SVM> svm = Algorithm::load<SVM>(g_SVM);
	Mat image;
	imageSrc.copyTo(image);
	vector<Mat> arr;
	getSegment(image, arr, image);
	Mat imageDataMat;
	getMixedFeature(arr[0], imageDataMat, Size(28, 28));
	int ans = (int)svm->predict(imageDataMat);

	return ans;
}

//基于图片的手写数字识别
void imageRecognize()
{
	string fileName;
	Mat srcImage;
	do {
		cout << "请将图片放置在程序文件目录下，并输入图片名称：" << endl;
		cin >> fileName;
		srcImage = imread(fileName);
		if (srcImage.empty())
		{
			cout << "未找到此图片，请重新输入" << endl;
		}
	} while (srcImage.empty());

	srcImage = imread(fileName);
	resize(srcImage, srcImage, Size(1024, 768), 0, 0, INTER_AREA);
	Mat dstImage = srcImage.clone();
	imshow("原图", srcImage);
	waitKey();
	preProcess(srcImage, dstImage);
	imshow("处理后", dstImage);
	waitKey(0);

	vector<Mat> arr;
	arr.clear();
	getSegment(dstImage, arr, srcImage);
	namedWindow(WINDOW_NAME);
	//resizeImage(srcImage, Size(800, 600));
	imshow(WINDOW_NAME, srcImage);

	Mat imageDataMat;
	Ptr<SVM> svm = Algorithm::load<SVM>(g_SVM);
	for (int i = 0; i < arr.size(); ++i)
	{
		Mat imageDataMat;
		getMixedFeature(arr[i], imageDataMat, Size(28, 28));
		imageDataMat.convertTo(imageDataMat, CV_32FC1);
		float ans = (float)svm->predict(imageDataMat);
		cout << ans << " ";
	}
	cout << endl;
	cout << "输入回车回到上一级" << endl;

	int time = waitKey(0);
	if (time == 13)
	{
		destroyWindow(WINDOW_NAME);
	}
}
