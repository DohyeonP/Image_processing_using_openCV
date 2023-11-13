#include "ISP.h"

ISP::ISP()
{
	std::cout << "ISP::Ctor" << std::endl;
}

ISP::~ISP()
{
	std::cout << "ISP::Dtor" << std::endl;
}

bool ISP::Convert_BGR2GRAY(uchar* pBGR, int cols, int rows, uchar* pGray)
{
	if (pBGR == nullptr || pGray == nullptr) return false;

	cv::Mat src = cv::Mat(rows, cols, CV_8UC3, pBGR);
	cv::Mat gray_cv = cv::Mat(rows, cols, CV_8UC1, pGray);

	cvtColor(src, gray_cv, COLOR_BGR2GRAY);

#if 0
	for (size_t row = 0; row < rows; row++)
	{
		for (size_t col = 0; col < cols; col++)
		{
			int index = row * cols + col;

			float val_Y =
				0.299 * pBGR[index * 3 + 2]		//r
				+ 0.587 * pBGR[index * 3 + 1]	//g
				+ 0.114 * pBGR[index * 3 + 0];	//b
			pGray[index] = (uchar)(val_Y + 0.5);
		}
	}
#endif
	return true;
}

bool ISP::Get_Histogram(uchar* pGray, int cols, int rows, int* pHisto, int histoSz)
{
	if (pGray == nullptr || pHisto == nullptr) return false;
#if 0
	for (size_t row = 0; row < rows; row++)
	{
		for (size_t col = 0; col < cols; col++)
		{
			int index = row * cols + col;
			if (pGray[index] >= histoSz) continue;
			pHisto[pGray[index]]++;
		}
	}
#else
	for (size_t value = 0; value < cols*rows; value++)
	{
		if (pGray[value] >= histoSz) continue;
		pHisto[pGray[value]]++;
	}
#endif
	return true;
}

bool ISP::Enhance_HistogramEq(uchar* pGray, int cols, int rows, uchar* pGrayEq)
{
	const int histoSz = 256;
	int length = cols * rows;
	int histogram[256] = { 0, };
	Get_Histogram(pGray, cols, rows, histogram, histoSz);

	// 확률밀도함수
	int acc[histoSz] = { 0, };
	acc[0] = histogram[0];

	for (size_t index = 1; index < histoSz; index++)
	{
		//histogram[i-1]
		acc[index] = acc[index - 1] + histogram[index];
	}

	// new Look Up Table ... 영상의 각 화소 값들을 새로운 대응값으로 맵핑
	int new_Gray[histoSz] = { 0, };
	//new_Grayp[i] = acc[i]*255 / length(cols*rows)
	for (size_t index = 0; index < histoSz; index++)
	{
		new_Gray[index] = static_cast<int>(1.0 * acc[index] * 255 / length);
	}

	for (size_t index = 0; index < length; index++)
	{
		pGrayEq[index] = new_Gray[pGray[index]];
	}

	return true;
}

bool ISP::Gamma_Correction(uchar* pGray, int cols, int rows, uchar* pGrayGm)
{
	int index;
	double normalized_value, corrected_value;

	if (pGray == nullptr || pGrayGm == nullptr) return false;

	for (size_t row = 0; row < rows; row++)
	{
		for (size_t col = 0; col < cols; col++)
		{
			index = row * cols + col;

			normalized_value = (double)pGray[index] / 255.0;
			corrected_value = pow(normalized_value, 1.5) * 255.0;

			pGrayGm[index] = static_cast<int>(corrected_value);
		}
	}

	return true;
}
