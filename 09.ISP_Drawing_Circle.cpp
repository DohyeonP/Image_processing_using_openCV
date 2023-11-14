#pragma once

#include "ISP.h"

int main() {
	std::string fileName = "../KCCImageNet/stop_img.png";
	cv::Mat src_color = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);
	cv::Mat src_HSV;
	cvtColor(src_color, src_HSV, COLOR_BGR2HSV);

	uchar* pData = src_HSV.data;
	size_t width = src_HSV.cols;
	size_t height = src_HSV.rows;

	cv::Mat src_color_bin = Mat::zeros(cv::Size(width, height), CV_8UC1);
	uchar* pDataBin = src_color_bin.data;

	int threshold_B = 50;
	int threshold_G = 30;
	int threshold_R = 180;

	// ¿Ã¡¯»≠, Binary
	for (size_t i = 0; i < width * height; i=i+3) {
		int B = pData[i];
		int G = pData[i + 1];
		int R = pData[i + 2];

		(B > threshold_B || G > threshold_G || R < threshold_R)
			? pDataBin[i] = 0 : pDataBin[i] = 255;
	}

	return 1;
}