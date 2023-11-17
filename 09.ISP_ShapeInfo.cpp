#pragma once

#include "ISP.h"

int main() {
	std::string fileName = "../KCCImageNet/images/brake_disk/brake_disk_part_02.png";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	uchar* pData = src_gray.data;
	size_t width = src_gray.cols;
	size_t height = src_gray.rows;

	cv::Mat src_bin = Mat::zeros(cv::Size(width, height), CV_8UC1);
	cv::Mat src_obj = Mat::zeros(cv::Size(width, height), CV_8UC1);
	uchar* pDataBin = src_bin.data;

	int threshold_min = 60;   // 0~255
	int threshold_max = 200;  // 0~255
	// 이진화, Binary
	for (size_t i = 0; i < width * height; i++) {
		int value = pData[i];

		// if (value > threshold_max)
		//{
		//	pDataBin[i] = 0;
		// }
		// else
		//	pDataBin[i] = 255;
		(value > threshold_max) ? pDataBin[i] = 0 : pDataBin[i] = 255;
	}
	src_obj = src_bin & src_gray;

	RNG rng(12345);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(src_bin, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	Mat drawing = Mat::zeros(src_bin.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++) {
		Scalar color =
			Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
	}

	Mat src_color;
	cv::cvtColor(src_gray, src_color, ColorConversionCodes::COLOR_GRAY2BGR);
	for (size_t i = 0; i < contours.size(); i++) {
		double area = contourArea(contours[i]);
		RotatedRect rrt = minAreaRect(contours[i]);
		double arcLen = arcLength(contours[i], true);

		Point ptTxt = Point(rrt.boundingRect().x, rrt.boundingRect().y);
		string msg;
		msg = std::format("area = {:.1f}", area);
		putText(src_color, msg, Point(ptTxt.x, ptTxt.y + 30 * 0),
			FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 8);
		msg = std::format("x,y = {:.1f}, {:.1f}", rrt.center.x, rrt.center.y);
		putText(src_color, msg, Point(ptTxt.x, ptTxt.y + 30 * 1),
			FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 8);
		msg = std::format("length = {:.1f}", arcLen);
		putText(src_color, msg, Point(ptTxt.x, ptTxt.y + 30 * 2),
			FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 8);
		cv::rectangle(src_color, rrt.boundingRect2f().tl(),
			rrt.boundingRect2f().br(), CV_RGB(0, 0, 255));
		cv::drawMarker(src_color, rrt.center, CV_RGB(255, 0, 0));

		const int ptSz = 4;
		Point2f pt[ptSz];
		rrt.points(pt);

		for (size_t i = 0; i < ptSz; i++) {
			cv::line(src_color, pt[i % ptSz], pt[(i + 1) % ptSz], CV_RGB(255, 0, 255),
				1);
		}

		continue;

		std::cout << "Object[" << i + 1 << "] 개수는 " << contours[i].size()
			<< " 입니다." << std::endl;
		int CoGx, CoGy;
		CoGx = CoGy = 0;
		int accX = 0, accY = 0;
		int length = contours[i].size();
		// add
		int x_min = width, x_max = 0, y_min = height, y_max = 0;
		for (size_t n = 0; n < length; n++) {
			accX += contours[i].at(n).x;
			accY += contours[i].at(n).y;
			if (x_min > contours[i].at(n).x) x_min = contours[i].at(n).x;
			if (x_max < contours[i].at(n).x) x_max = contours[i].at(n).x;
			if (y_min > contours[i].at(n).y) y_min = contours[i].at(n).y;
			if (y_max < contours[i].at(n).y) y_max = contours[i].at(n).y;
		}
		CoGx = accX / length;
		CoGy = accY / length;
		std::cout << "Object[" << i + 1 << "] CoG.x = " << CoGx
			<< " CoG.y = " << CoGy << std::endl;

		cv::line(src_color, Point(CoGx - 10, CoGy - 10),
			Point(CoGx + 10, CoGy + 10), CV_RGB(255, 0, 0), 10);
		cv::line(src_color, Point(CoGx + 10, CoGy - 10),
			Point(CoGx - 10, CoGy + 10), CV_RGB(255, 0, 0), 10);

		// const int ptSz = 4;
		// Point pt[ptSz];
		pt[0].x = x_min;
		pt[0].y = y_min;  // 좌상
		pt[1].x = x_max;
		pt[1].y = y_min;  // 우상
		pt[2].x = x_max;
		pt[2].y = y_max;  // 우하
		pt[3].x = x_min;
		pt[3].y = y_max;  // 좌하

		for (size_t i = 0; i < ptSz; i++)
		{
			cv::line(src_color, pt[i % ptSz], pt[(i + 1) % ptSz], CV_RGB(0, 0, 255), 1);
		}
	}
	return 1;
}
