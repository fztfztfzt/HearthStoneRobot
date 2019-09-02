#include "OutputInfo.h"
//#include<Windows.h>

using namespace std;

COutputInfo::COutputInfo(std::string outputFile)
	:m_coutFile(outputFile.c_str())
{
	/*HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rc = { 1400, 0, 120, 30 };
	SetConsoleWindowInfo(hOut, true, &rc);*/

	m_pCoutBuf = cout.rdbuf();
	streambuf* fileBuf = m_coutFile.rdbuf();
	cout.rdbuf(fileBuf);
}

COutputInfo::~COutputInfo()
{
	m_coutFile.flush();
	m_coutFile.close();
	cout.rdbuf(m_pCoutBuf);
}
