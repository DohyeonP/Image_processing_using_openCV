#pragma once

#include "ISP.h"

int main()
{
	std::string fileName = "../KCCImageNet/stop_img.png";
	cv::Mat src_color_original = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);;
	cv::Mat src_HSV;
	cvtColor(src_color_original, src_HSV, COLOR_BGR2HSV);

	uchar* pData_HSV = src_HSV.data;
	size_t width = src_HSV.cols;
	size_t height = src_HSV.rows;

	cv::Mat src_HSV_bin = Mat::zeros(cv::Size(width, height), CV_8UC1);

	uchar* pData_HSV_bin = src_HSV_bin.data;

	int threshold_V_min = 170;
	int threshold_V_max = 180;
	int threshold_S_min = 180;
	int threshold_S_max = 220;
	int threshold_H_min = 180;
	int threshold_H_max = 255;

	int brightness = 0;
	float avg_brightness;
	int count = 0;

	// 이진화, Binary
	for (size_t index = 0; index < width * height; index++)
	{
		int HSV = index * src_HSV.channels();
		int H = pData_HSV[HSV + 2];
		int S = pData_HSV[HSV + 1];
		int V = pData_HSV[HSV + 0];

		brightness += V;
		count++;

		((threshold_H_min < H && H < threshold_H_max) &&
			(threshold_S_min < S && S < threshold_S_max) &&
			(threshold_V_min < V && V < threshold_V_max) ||
			((0 <= V && V <= 2) && (145 <= S && S <= 200) && (170 <= H && H <= 223))) ?
			pData_HSV_bin[index] = 255 : pData_HSV_bin[index] = 0;
	}

	avg_brightness = brightness / count;

	RNG rng(12345);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(src_HSV_bin, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	Mat drawing = Mat::zeros(src_HSV_bin.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++) {
		Scalar color =
			Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours, (int)i, color, 1, LINE_8, hierarchy, 0);
	}
	
	Mat src_color = src_color_original.clone();
//	cv::cvtColor(src_gray, src_color, ColorConversionCodes::COLOR_GRAY2BGR);
	for (size_t i = 0; i < contours.size(); i++)
	{
		const int ptSz = 4;
		Point2f pt[ptSz];

		// 여기서 부터 내접하는 도형 그리는 부분
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
		int radius = (y_max - y_min) / 2;
		CoGx = accX / length;
		CoGy = accY / length;
		std::cout << "Object[" << i + 1 << "] CoG.x = " << CoGx
			<< " CoG.y = " << CoGy << std::endl;

		// 중심점
		cv::line(src_color, Point(CoGx, CoGy),
			Point(CoGx, CoGy), CV_RGB(80, 120, 160), 2);

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

		// 외접하는 도형 그리기
		for (size_t i = 0; i < ptSz; i++)
		{
			// 사각형
			cv::line(src_color, pt[i % ptSz], pt[(i + 1) % ptSz], CV_RGB(0, 0, 0), 1);
			// 각 꼭짓점을 잇는 직선 그리기
			cv::line(src_color, pt[i % ptSz], pt[(i + 2) % ptSz], CV_RGB(0, 0, 0), 1);
		}
		// 사각형 윗변 아랫변에 내접하는 원 그리기
		cv::circle(src_color, Point(114, 113), radius, CV_RGB(0, 0, 0), 1);

		// 도형의 정보 출력하는 부분
		double area = contourArea(contours[i]);
		RotatedRect rrt = minAreaRect(contours[i]);
		double arcLen = arcLength(contours[i], true);

		rrt.points(pt);

		for (size_t i = 0; i < ptSz; i++) {
			cv::line(src_color, pt[i % ptSz], pt[(i + 1) % ptSz], CV_RGB(255, 0, 255), 1);
		}

		Point ptTxt = Point(rrt.boundingRect().x, rrt.boundingRect().y);
		string msg;
		// contours 갯수
		msg = std::format("area = {:.1f}", area);
		putText(src_color, msg, Point(x_max + 10, y_min + 20 * 0),
			FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(10, 0, 10), 1, 8);
		// 사각형의 중심점
		msg = std::format("x,y = {:.1f}, {:.1f}", rrt.center.x, rrt.center.y);
		putText(src_color, msg, Point(x_max + 10, y_min + 20 * 1),
			FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(10, 0, 10), 1, 8);
		// 입력 곡선 길이
		msg = std::format("length = {:.1f}", arcLen);
		putText(src_color, msg, Point(x_max + 10, y_min + 20 * 2),
			FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(10, 0, 10), 1, 8);
		// 반지름
		msg = std::format("radius = {:d}", radius);
		putText(src_color, msg, Point(x_max + 10, y_min + 20 * 3),
			FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(10, 0, 10), 1, 8);
		// 밝기
		msg = std::format("Avg_brightness = {:.1f}", avg_brightness);
		putText(src_color, msg, Point(x_max + 10, y_min + 20 * 4),
			FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(10, 0, 10), 1, 8);
		cv::rectangle(src_color, rrt.boundingRect2f().tl(),
			rrt.boundingRect2f().br(), CV_RGB(0, 0, 255));
		cv::drawMarker(src_color, rrt.center, CV_RGB(255, 0, 0), 3);
	}
	
	return 1;
}