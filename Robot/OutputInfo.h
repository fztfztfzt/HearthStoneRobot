#pragma once

#include <iostream>
#include <fstream>
#include <xstring>

class COutputInfo
{
public:
	COutputInfo(std::string outputFile = "cout.txt");

	~COutputInfo();

private:
	std::streambuf* m_pCoutBuf = nullptr;
	std::ofstream m_coutFile;
};

