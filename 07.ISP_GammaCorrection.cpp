#pragma once

#include "ISP.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);

	uchar* pData = src.data;
	int length = src.total();
	int channels = src.channels();

	ISP _isp;

	//origin
	cv::Mat gray = cv::Mat(src.rows, src.cols, CV_8UC1);

	//enhanced_image using HistoEq
	cv::Mat gray_Eq = cv::Mat(src.rows, src.cols, CV_8UC1);

	//gamma_correction
	cv::Mat gray_Gm = cv::Mat(src.rows, src.cols, CV_8UC1);

	_isp.Convert_BGR2GRAY(src.data, src.cols, src.rows, gray.data);
	_isp.Enhance_HistogramEq(gray.data, gray.cols, gray.rows, gray_Eq.data);

	_isp.Gamma_Correction(gray.data, gray.cols, gray.rows, gray_Gm.data);

	return 1;
}