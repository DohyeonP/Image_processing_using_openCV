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

	cv::Mat gray_cv = cv::Mat(src.rows, src.cols, CV_8UC1);

	_isp.Convert_BGR2GRAY(src.data, src.cols, src.rows, gray.data);
	cv::cvtColor(src, gray_cv, ColorConversionCodes::COLOR_BGR2GRAY);
	_isp.Enhance_HistogramEq(gray.data, gray.cols, gray.rows, gray_Eq.data);

	// Library OpenCV
	// check correlation
	{
		cv::Mat gray_cv;
		cv::Mat gray_Eq_cv;
		cvtColor(src, gray_cv, COLOR_BGR2GRAY);
		cv::equalizeHist(gray_cv, gray_Eq_cv);

		cv::Mat diff = gray_Eq - gray_Eq_cv;

		double error = cv::sum(diff)[0];
		int a = 0;

		Scalar sum = cv::sum(diff);
	}

	return 1;
}