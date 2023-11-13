#pragma once
#include "Common.h"

class ISP
{
public:
	ISP();
	~ISP();

	bool Convert_BGR2GRAY(
		uchar* pBGR,// color channel, input
		int cols,	// width, input
		int rows, // height, input
		uchar* pGray// mono channel, output
	);

	bool Get_Histogram(
		uchar* pGray,
		int cols,	// cols, input
		int rows,	// rows, input
		int* pHisto,
		int histoSz
	);

	bool Enhance_HistogramEq(
		uchar* pGray,
		int cols,	// cols, input
		int rows,	// rows, input
		uchar* pGrayEq
	);

	bool Gamma_Correction(
		uchar* pGray,
		int cols,
		int rows,
		uchar* pGrayGm
	);
private:
};