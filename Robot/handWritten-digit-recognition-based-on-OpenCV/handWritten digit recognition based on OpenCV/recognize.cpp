#include"recognize.h"

//���ڿ���̨����д����ʶ��
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
		cout << "Ԥ������" << endl;
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

		cout << "���������ַ�������һ�ε����룬����esc�ص���һ��" << endl;
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

//����ͼƬ����д����ʶ��
void imageRecognize()
{
	string fileName;
	Mat srcImage;
	do {
		cout << "�뽫ͼƬ�����ڳ����ļ�Ŀ¼�£�������ͼƬ���ƣ�" << endl;
		cin >> fileName;
		srcImage = imread(fileName);
		if (srcImage.empty())
		{
			cout << "δ�ҵ���ͼƬ������������" << endl;
		}
	} while (srcImage.empty());

	srcImage = imread(fileName);
	resize(srcImage, srcImage, Size(1024, 768), 0, 0, INTER_AREA);
	Mat dstImage = srcImage.clone();
	imshow("ԭͼ", srcImage);
	waitKey();
	preProcess(srcImage, dstImage);
	imshow("�����", dstImage);
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
	cout << "����س��ص���һ��" << endl;

	int time = waitKey(0);
	if (time == 13)
	{
		destroyWindow(WINDOW_NAME);
	}
}
