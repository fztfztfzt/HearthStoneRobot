#include <opencv2\opencv.hpp>
#include <fstream>
#include<vector>
using namespace std;
using namespace cv;
/** @主函数 */
const int MAXV = 1;
fstream f;
int num[11][9] = { { 0, 1, 1, 1, 0, 1, 1, 1, 1 }, { 0, 1, 1, 0, 1, 1, 0, 1, 1 }, { 1, 0, 1, 1, 0, 1, 0, 1, 0 }, { 0, 1, 0, 0, 1, 1, 0, 0, 1 },
{ 0, 0, 1, 0, 0, 1, 0, 1, 1 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 1, 0, 1, 1, 1, 1 }, { 1, 1, 1, 0, 1, 0, 0, 1, 0 },
{ 0, 0, 1, 0, 1, 1, 1, 0, 0 }, { 1, 1, 1, 1, 0, 1, 0, 0, 0 }, { 0, 1, 1, 1, 0, 1, 1, 1, 0 } };
void find(Mat src)//使用图像查找方法
{
	Mat des = imread("HS/tep_2.bmp", 1);
	Mat image = imread("HS/tpl.bmp", 1);
	Mat tpl[16];
	for (int i = 0; i < 4;++i)
	for (int j = 0; j < 4; ++j)
	{
		image(Rect(j * 45, i * 45, 45, 45)).copyTo(tpl[i * 4 + j]);
		//imshow("tpl", tpl[i*4+j]);
		//waitKey(0);
	}
	tpl[0] = imread("HS/0.bmp", 1);
	tpl[1] = imread("HS/1.bmp", 1);
	tpl[2] = imread("HS/2.bmp", 1);
	tpl[3] = imread("HS/3.bmp", 1);
	int resultsum = 0, ri = 0, rj = 0,rn=0;
	for (int k = 0; k < 14; ++k)
	{
		int maxsum = 0, maxi = 0, maxj = 0;
		imshow("tpl", tpl[k]);
		for (int i = 0; i < src.rows; i+=2)
		{
			for (int j = 0; j<src.cols; j+=2)
			{
				int sum = 0;
				for (int m = 0; src.rows - i>tpl[k].rows&& m < tpl[k].rows; m++)
				{
					for (int n = 0; src.cols - j>tpl[k].cols&& n < tpl[k].cols; n++)
					{
						uchar r = src.at<Vec3b>(i + m, j + n)[0];
						uchar g = src.at<Vec3b>(i + m, j + n)[1];
						uchar b = src.at<Vec3b>(i + m, j + n)[2];

						uchar r2 = tpl[k].at<Vec3b>(m, n)[0];
						uchar g2 = tpl[k].at<Vec3b>(m, n)[1];
						uchar b2 = tpl[k].at<Vec3b>(m, n)[2];
					//	cout << r2 << " " << g2 << " " << b2 << endl;
						if (r2+g2+b2!=255*3 && abs(r - r2) < MAXV && abs(g - g2) < MAXV && abs(b - b2) < MAXV)
						{
							sum++;
						}
					}
				}
				if (sum>maxsum)
				{
					maxsum = sum;
					maxi = i;
					maxj = j;
				}
				/*if (src.cols - j > tpl[k].cols && src.rows - i > tpl[k].rows)
				{
					Rect r(i, j, tpl[rn].rows, tpl[rn].cols);
					Mat temp;
					src(r).copyTo(temp);
					cout << sum << endl;
					imshow("费用", temp);
					waitKey(0);
				}*/
			}
		}
		cout << "num:" << k << "maxsum:" << maxsum << endl;
		if (maxsum > resultsum)
		{
			resultsum = maxsum;
			ri = maxi;
			ri = maxj;
			rn = k;
		}
	}
	Rect r(ri, ri, tpl[rn].rows, tpl[rn].cols);
	Mat temp;
	src(r).copyTo(temp);
	cout << "result::num:" << rn << ";max:" << resultsum << endl;
	imshow("费用", temp);
	waitKey(0);
}
void recoNum(Mat src)//数字识别
{
	Mat sub[9];
	int h = src.rows / 3;
	int w = src.cols / 3;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			src(Rect(j*w, i*h, w, h)).copyTo(sub[i * 3 + j]);
			/*imshow("sub", sub[i*3 + j]);
			waitKey(0);*/
		}
	}
	int temp[9] = { 0 };
	for (int i = 0; i < 9; ++i)
	{
		int num = 0;
		for (int j = 0; j < sub[i].cols; ++j)
		{
			for (int k = 0; k < sub[i].rows; ++k)
			{
				if (sub[i].at<uchar>(k, j) == 255)
				{
					num++;
				}
			}
		}
		//cout << num << " ";
		if (num>w*h / 2) temp[i] = 1, cout << "1" <<" ";
		else cout << "0" << " ";
		if (i % 3 == 2) f << endl;
	}
	int maxsum = 0,maxn=0;
	for (int i = 0; i < 11; ++i)
	{
		int sum = 0;
		for (int j = 0; j < 9; ++j)
		{
			if (temp[j] == num[i][j])
			{
				++sum;
			}
		}
		if (sum>maxsum)
		{
			maxsum = sum;
			maxn = i;
		}
	}
	cout << "识别出的数字是："<<maxn << endl;
}
void find2(Mat src)//提取数字部分轮廓
{
	Mat image;
	src(Rect(0, 0, src.rows / 4, src.cols / 3)).copyTo(image);
	imshow("windows",image);
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);
	Mat thr;
	threshold(gray, thr, 250, 255, CV_THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat cz;
	dilate(thr, cz, element);
	erode(cz, cz, element);
	/*imshow("gray2", cz);
	waitKey(0);*/
	vector<std::vector<Point>> contours;
	findContours(cz,			//图像
		contours,				//轮廓点
		//包含图像拓扑结构的信息（可选参数，这里没有选）
		CV_RETR_EXTERNAL,			//获取轮廓的方法（这里获取外围轮廓）
		CV_CHAIN_APPROX_NONE);		//轮廓近似的方法（这里不近似，获取全部轮廓）
	//打印轮廓信息
	std::cout << "共有外围轮廓：" << contours.size() << "条" << std::endl;
	Rect r0 = boundingRect(Mat(contours[0]));
	Mat matio;
	thr(r0).copyTo(matio);
	rectangle(src, r0, Scalar(0), 2);
	//imshow("222" , matio);
	//waitKey(0);
	recoNum(matio);
	f << endl;
}
void preprocess()//预处理，得到数字0-10的模型
{
	f.open("result.txt", ios::out);
	for (int i = 0; i < 11; ++i)
	{
		stringstream ss;
		ss << "HS/tpl_" << i << ".bmp";
		string s = ss.str();
		Mat tpl = imread(s, 1);
		f << i << endl;
		find2(tpl);
		waitKey(0);
	}
	f.close();
}
void process()//预处理发牌阶段图像
{
	/// 读入原图像, 返回3通道图像数据
	Mat src = imread("HS/test.bmp", 1);
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	Mat thr;
	threshold(gray, thr, 50, 255, CV_THRESH_BINARY);
	imshow("gray", thr);
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	dilate(thr, thr, element);
	erode(thr, thr, element);
	//erode(thr, thr, element);

	imshow("fushi", thr);
	waitKey(0);
	vector<std::vector<Point>> contours;
	findContours(thr,			//图像
		contours,				//轮廓点
		//包含图像拓扑结构的信息（可选参数，这里没有选）
		CV_RETR_EXTERNAL,			//获取轮廓的方法（这里获取外围轮廓）
		CV_CHAIN_APPROX_NONE);		//轮廓近似的方法（这里不近似，获取全部轮廓）
	//打印轮廓信息
	std::cout << "共有外围轮廓：" << contours.size() << "条" << std::endl;
	std::vector<std::vector<Point>>::const_iterator itContours = contours.begin();
	for (; itContours != contours.end(); ++itContours)
	{
		std::cout << "每个轮廓的长度: " << itContours->size() << std::endl;
	}




	Mat result(thr.size(), CV_8U, Scalar(255));
	result.setTo(Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 2);
	imshow("提取外围轮廓", result);

	//除去太长或者太短的轮廓
	int cmin = 700;
	int cmax = 900;
	std::vector<std::vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if (itc->size() < cmin || itc->size() > cmax)
			itc = contours.erase(itc);
		else
			++itc;
	}

	//画出轮廓

	//画出轮廓，参数为：画板，轮廓，轮廓指示（这里画出所有轮廓），颜色，线粗
	//	drawContours(result, contours, -1, Scalar(0), 2);
	//将轮廓重绘于白板上
	result.setTo(Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 1);
	imshow("去除较小的", result); //还要注意提取轮廓的方法还有很多种，比如CV_RETR_LIST代表所有轮廓
	waitKey(0);
	//轮廓的形状描述子
	//外接矩形
	Mat matio[10];
	int num = contours.size();
	for (int i = 0; i < contours.size(); ++i)
	{
		Rect r0 = boundingRect(Mat(contours[i]));
		src(r0).copyTo(matio[i]);
		rectangle(src, r0, Scalar(0), 2);
		imshow("" + i, matio[i]);
		find2(matio[i]);
	}
	imshow("外接矩形", src);

	//	findContours(thr,			//图像 
	//	contours,				//轮廓点
	//	//包含图像拓扑结构的信息（可选参数，这里没有选）
	//	CV_RETR_LIST,			//获取轮廓的方法（这里获取所有轮廓）
	//	CV_CHAIN_APPROX_NONE);		//轮廓近似的方法（这里不近似，获取全部轮廓
	////画出轮廓
	//drawContours(result, contours, -1, Scalar(0), 2);
	//imshow("提取所有轮廓", result);
}
void process2()//处理出牌阶段场景
{
	/// 读入原图像, 返回3通道图像数据
	Mat src = imread("HS/secene_1.bmp", 1);
	Mat image;
	src.copyTo(image);
	for (int i = 0; i<src.rows; i++)//阈值处理，白色留下，其余改为黑色
	{
		for (int j = 0; j<src.cols; j++)
		{
			uchar r = src.at<Vec3b>(i, j)[0];
			uchar g = src.at<Vec3b>(i, j)[1];
			uchar b = src.at<Vec3b>(i, j)[2];
			if (r == 255 && g == 255 && b == 255)
			{
				image.at<Vec3b>(i, j)[0] = 255;
				image.at<Vec3b>(i, j)[1] = 255;
				image.at<Vec3b>(i, j)[2] = 255;
			}
			else
			{
				image.at<Vec3b>(i, j)[0] = 0;
				image.at<Vec3b>(i, j)[1] = 0;
				image.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);//转换为单通道便于进行轮廓提取
	imshow("gray", gray);

	//Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));//腐蚀膨胀效果不好
	//dilate(thr, thr, element);
	//erode(thr, thr, element);
	//dilate(thr, thr, element);
	//erode(thr, thr, element);
	//imshow("fushi", thr);
	vector<std::vector<Point>> contours;
	findContours(gray,			//图像
		contours,				//轮廓点
		//包含图像拓扑结构的信息（可选参数，这里没有选）
		CV_RETR_EXTERNAL,			//获取轮廓的方法（这里获取外围轮廓）
		CV_CHAIN_APPROX_NONE);		//轮廓近似的方法（这里不近似，获取全部轮廓）
	//打印轮廓信息
	std::cout << "共有外围轮廓：" << contours.size() << "条" << std::endl;
	std::vector<std::vector<Point>>::const_iterator itContours = contours.begin();
	for (; itContours != contours.end(); ++itContours)
	{
		std::cout << "每个轮廓的长度: " << itContours->size() << std::endl;
	}




	Mat result(gray.size(), CV_8U, Scalar(255));
	result.setTo(Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 2);
	imshow("提取外围轮廓", result);

	//除去太长或者太短的轮廓
	int cmin = 10;
	int cmax = 100;
	std::vector<std::vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if (itc->size() < cmin || itc->size() > cmax)
			itc = contours.erase(itc);
		else
			++itc;
	}

	//画出轮廓，参数为：画板，轮廓，轮廓指示（这里画出所有轮廓），颜色，线粗
	//	drawContours(result, contours, -1, Scalar(0), 2);
	//将轮廓重绘于白板上
	result.setTo(Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 1);
	imshow("去除较小的", result); //还要注意提取轮廓的方法还有很多种，比如CV_RETR_LIST代表所有轮廓
	waitKey(0);
	//轮廓的形状描述子
	//外接矩形
	Mat matio[100];
	int num = contours.size();
	for (int i = 0; i < contours.size(); ++i)
	{
		Rect r0 = boundingRect(Mat(contours[i]));
		if (r0.width >= r0.height) continue;
		src(r0).copyTo(matio[i]);
		rectangle(src, r0, Scalar(0), 2);
		imshow("iiiiiiiii", matio[i]);
		//find2(matio[i]);
	}
	imshow("外接矩形", src);

	//	findContours(thr,			//图像 
	//	contours,				//轮廓点
	//	//包含图像拓扑结构的信息（可选参数，这里没有选）
	//	CV_RETR_LIST,			//获取轮廓的方法（这里获取所有轮廓）
	//	CV_CHAIN_APPROX_NONE);		//轮廓近似的方法（这里不近似，获取全部轮廓
	////画出轮廓
	//drawContours(result, contours, -1, Scalar(0), 2);
	//imshow("提取所有轮廓", result);
}
int main(int argc, char** argv)
{
	process2();
	waitKey(0);
	return(0);
}