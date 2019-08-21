#include "opencv2/imgproc/types_c.h"
#include"preprocess.h"

void preProcess(const Mat& srcImage, Mat& dstImage)
{
	Mat tmpImage = srcImage.clone();
	if (tmpImage.type() != CV_8UC1)
	{
		cvtColor(tmpImage, tmpImage, CV_BGR2GRAY);
	}
	//��ֵ�˲�Ԥ�������
	//GaussianBlur(tmpImage, tmpImage, Size(3, 3), 0, 0, BORDER_DEFAULT);
	medianBlur(tmpImage, tmpImage, 3);
	//��̬ѧ�˲� ������Ԥ����
	Mat element = getStructuringElement(MORPH_RECT, Size(7,7));
	//morphologyEx(tmpImage, tmpImage, MORPH_OPEN, element);

	//imshow("��˹�˲��ӿ�����", tmpImage);
	//waitKey(500);
	
	//canny��ȡ����
	Canny(tmpImage, dstImage, 75, 100, 3);
	//imshow("canny������ȡ", dstImage);
	//waitKey(500);

	//��̬ѧ�˲����������մ���
	element = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(dstImage, dstImage, MORPH_DILATE, element);
	//imshow("��̬ѧ����", tmpImage4);
	//waitKey(500);

	//��ֵ�˲����������
	//medianBlur(tmpImage4, dstImage, 3);
	//imshow("��ֵ�˲���", dstImage);
	//waitKey(500);
}