#include<Windows.h>
#include<assert.h>
#include<fstream>
#include<iostream>
using namespace std;
bool readBmp(char *bmpName, unsigned char *&pBmpBuf, LONG &bmpWidth, LONG &bmpHeight)
{
	FILE *fp = fopen(bmpName, "rb");//二进制读方式打开指定的图像文件

	if (fp == 0)
		return 0;

	//跳过位图文件头结构BITMAPFILEHEADER

	fseek(fp, sizeof(BITMAPFILEHEADER), 0);

	//定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中

	BITMAPINFOHEADER head;

	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp); //获取图像宽、高、每像素所占位数等信息

	bmpWidth = head.biWidth;

	bmpHeight = abs(head.biHeight);

	WORD biBitCount = head.biBitCount;//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）

	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;//灰度图像有颜色表，且颜色表表项为256

	//申请位图数据所需要的空间，读位图数据进内存

	pBmpBuf = new unsigned char[lineByte * bmpHeight];

	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);

	fclose(fp);//关闭文件

	return 1;//读取文件成功
}
void findSubBmp()//逐像素判断RGB
{
	unsigned char *src = NULL, *des = NULL;
	LONG srcWidth, srcHeight, desWidth, desHeight;
	readBmp("src.bmp", src, srcWidth, srcHeight);
	readBmp("des.bmp", des, desWidth, desHeight);
	int srclineByte = (srcWidth * 24 / 8 + 3) / 4 * 4;
	int deslineByte = (desWidth * 24 / 8 + 3) / 4 * 4;
	int maxValue = 10, sum = 0, maxv = 0, maxi = 0, maxj = 0, maxv2 = 0, maxi2 = 0, maxj2 = 0;
	for (int i = 0; i < srcHeight; i += 8)
	{
		for (int j = 0; j < srcWidth; j += 8)
		{
			sum = 0;
			for (int m = 0; srcHeight - i>desHeight && m < desHeight; ++m)
			{
				for (int n = 0; srcWidth - j>desWidth && n < desWidth; ++n)
				{
					unsigned char b = src[(i + m)*srclineByte + (j + n) * 3];
					unsigned char g = src[(i + m)*srclineByte + (j + n) * 3 + 1];
					unsigned char r = src[(i + m)*srclineByte + (j + n) * 3 + 2];

					unsigned char b2 = des[(m)*deslineByte + (n)* 3];
					unsigned char g2 = des[(m)*deslineByte + (n)* 3 + 1];
					unsigned char r2 = des[(m)*deslineByte + (n)* 3 + 2];

					if (abs(b - b2) < maxValue && abs(g - g2) < maxValue && abs(r - r2) < maxValue) sum++;
				}
			}
			if (sum>maxv)
			{
				maxv = sum;
				maxi = i;
				maxj = j;
			}
			else if (sum>maxv2)
			{
				maxv2 = sum;
				maxi2 = i;
				maxj2 = j;
			}
		}
	}
	cout << maxv << " " << maxj << " " << maxi << endl;
	cout << maxv2 << " " << maxj2 << " " << maxi2 << endl;
	HDC hdc = ::GetDC(NULL);
	for (int i = maxi; i < maxi + desHeight; ++i)
	{
		for (int j = maxj; j < maxj + desWidth; ++j)
		{
			unsigned char b = src[i*srclineByte + j * 3];
			unsigned char g = src[i*srclineByte + j * 3 + 1];
			unsigned char r = src[i*srclineByte + j * 3 + 2];
			COLORREF c = RGB(r, g, b);
			::SetPixel(hdc, j - maxj, maxi + desHeight - i, c);
		}
	}
}


int main()
{
	findSubBmp();
	system("pause");
}