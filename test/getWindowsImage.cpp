#include<Windows.h>
#include<assert.h>
#include<fstream>
#include<iostream>
using namespace std;
void SaveHwndToBmpFile(HWND hWnd, LPCTSTR lpszPath)
{
	HDC hDC = ::GetWindowDC(hWnd);
	if (hDC == NULL) return;

	HDC hMemDC = ::CreateCompatibleDC(hDC);
	assert(hMemDC);

	RECT rc;
	::GetWindowRect(hWnd, &rc);

	HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);
	assert(hBitmap);

	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, hBitmap);

	::PrintWindow(hWnd, hMemDC, PW_CLIENTONLY);
	OpenClipboard(NULL);   EmptyClipboard();    SetClipboardData(CF_BITMAP, hMemDC);    CloseClipboard();
	BITMAP bitmap = { 0 };
	::GetObject(hBitmap, sizeof(BITMAP), &bitmap);
	BITMAPINFOHEADER bi = { 0 };
	BITMAPFILEHEADER bf = { 0 };

	CONST int nBitCount = 24;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bitmap.bmWidth;
	bi.biHeight = bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = nBitCount;
	bi.biCompression = BI_RGB;
	DWORD dwSize = ((bitmap.bmWidth * nBitCount + 31) / 32) * 4 * bitmap.bmHeight;
	unsigned char *data = new unsigned char[dwSize];

	::GetDIBits(hMemDC, hBitmap, 0, bitmap.bmHeight, data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
	
	//将图像直接画在桌面，便于观看
	/*HDC hdc = ::GetDC(NULL);
	int ww = ((bitmap.bmWidth * nBitCount + 31) / 32) * 4;
	for (int i = 0; i < bitmap.bmHeight; ++i)
	{
		for (int j = 0; j < bitmap.bmWidth; ++j)
		{
			unsigned char b = data[i*ww + j * 3];
			unsigned char g = data[i*ww + j * 3 + 1];
			unsigned char r = data[i*ww + j * 3 + 2];
			COLORREF c = RGB(r, g, b);
			::SetPixel(hdc, j, bitmap.bmHeight-i, c);
		}
	}*/
	//保存为bmp格式图像，便于查看
	{
		bf.bfType = 0x4d42;
		//dwSize += sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
		bf.bfSize = dwSize + sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
		bf.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);


		HANDLE hFile = CreateFile(lpszPath, GENERIC_WRITE, 0, NULL,

			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD dwWritten = 0;
		// Write the File header
		WriteFile(hFile, &bf, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
		// Write the bitmap info header
		WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
		WriteFile(hFile, data, dwSize, &dwWritten, NULL);
		// Close the file handle
		CloseHandle(hFile);

	//使用改方式保存会出现问题，图像显示不正常，未解决
		//ofstream file;
		//file.open("1.bmp");
		//file.write((char*)&bf, sizeof(BITMAPFILEHEADER));
		//file.write((char*)lpbi, dwSize);
		////file.write((char*)lpbi, sizeof(BITMAPINFOHEADER));
		//
		////file.write((char*)data, dwSize);
		//file.close();
		
	}

	::SelectObject(hMemDC, hOldBmp);
	::DeleteObject(hBitmap);
	::DeleteObject(hMemDC);
	::ReleaseDC(hWnd, hDC);
}
int main()
{
	HWND hWnd = ::FindWindow("UnityWndClass", ("炉石传说"));
	if (hWnd)
	{
		SaveHwndToBmpFile(hWnd, ("currentWindow.bmp"));
	}
}