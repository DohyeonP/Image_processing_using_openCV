#pragma once

#include "ISP.h"

int main()
{
	std::string fileName = "../KCCImageNet/stop_img.png";
	cv::Mat src_color = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);

	Mat src_hsv;
	cvtColor(src_color, src_hsv, COLOR_BGR2HSV);

	Mat src_hsv_bin, src_hsv_bin_Red_1, src_hsv_bin_Red_2;
	inRange(src_hsv, Scalar(160, 180, 0), Scalar(180, 255, 255), src_hsv_bin_Red_1);
	inRange(src_hsv, Scalar(0, 180, 0), Scalar(10, 255, 255), src_hsv_bin_Red_2);
	src_hsv_bin = src_hsv_bin_Red_1 + src_hsv_bin_Red_2;

	// Morpology
	int kernelSz = 2;
	int shape = MorphShapes::MORPH_ELLIPSE;
	cv::Size sz = Size(2 * kernelSz + 1, 2 * kernelSz + 1);
	Mat SE = cv::getStructuringElement(shape, sz);
	Mat src_opening, opening_2nd, opening_3rd, src_closeing;

	//closing
	int type = MorphTypes::MORPH_CLOSE;
	cv::morphologyEx(src_hsv_bin, src_closeing, type, SE);
	//end closing

	//opening
	type = MorphTypes::MORPH_OPEN;
	shape = MorphShapes::MORPH_CROSS;
	kernelSz = 1;
	sz = Size(2 * kernelSz + 1, 2 * kernelSz + 1);
	SE = cv::getStructuringElement(shape, sz);
	cv::morphologyEx(src_closeing, src_opening, type, SE);
	//end opening

	//opening 2nd
	sz = Size(2 * kernelSz + 1, 4 * kernelSz + 1);
	SE = cv::getStructuringElement(shape, sz);
	cv::morphologyEx(src_opening, opening_2nd, type, SE);
	//end opening 2nd

	//opening 3rd
	shape = MorphShapes::MORPH_CROSS;
	kernelSz = 1;
	sz = Size(2 * kernelSz, 2 * kernelSz);
	SE = cv::getStructuringElement(shape, sz);
	cv::morphologyEx(opening_2nd, opening_3rd, type, SE);
	//end opening 3rd

	RNG rng(12345);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(src_hsv_bin, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	Mat drawing = src_color.clone();// Mat::zeros(src_color.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		//drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
		drawContours(drawing, contours, (int)i, color, CV_FILLED);
	}

	//find max area object
	double max_area = 0;
	size_t max_area_index = 0;
	for (size_t i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		if (max_area < area)
		{
			max_area = area;
			max_area_index = i;
		}
	}
	drawContours(drawing, contours, (int)max_area_index, CV_RGB(255, 0, 0), CV_FILLED);

	const Point* pts = (const cv::Point*)Mat(contours[max_area_index]).data;
	int npts = Mat(contours[max_area_index]).rows;
	cv::polylines(drawing, &pts, &npts, 1, true, Scalar(200, 200, 200));
	cv::fillConvexPoly(drawing, pts, npts, cv::Scalar(0, 255, 200));

	//for (size_t i = 0; i < contours.size(); i++)
	{
		size_t i = max_area_index;
		double area = contourArea(contours[i]);
		RotatedRect rrt = minAreaRect(contours[i]);
		double arcLen = arcLength(contours[i], true);

		Point ptTxt = Point(rrt.boundingRect().br().x+10, rrt.boundingRect().y);
		string msg;
		msg = std::format("area = {:.1f}", area);
		putText(drawing, msg, Point(ptTxt.x, ptTxt.y + 30 * 0), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 8);
		msg = std::format("x,y = {:.1f}, {:.1f}", rrt.center.x, rrt.center.y);
		putText(drawing, msg, Point(ptTxt.x, ptTxt.y + 30 * 1), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 8);
		msg = std::format("length = {:.1f}", arcLen);
		putText(drawing, msg, Point(ptTxt.x, ptTxt.y + 30 * 2), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 8);
		cv::rectangle(drawing, rrt.boundingRect2f().tl(), rrt.boundingRect2f().br(), CV_RGB(0, 0, 255));
		cv::drawMarker(drawing, rrt.center, CV_RGB(255, 0, 0));
		cv::ellipse(drawing, rrt, CV_RGB(10, 255, 10));

		int radius = std::min(rrt.boundingRect().width, rrt.boundingRect().height);
		radius /= 2;
		cv::circle(drawing, rrt.center, radius, CV_RGB(10, 50, 255));

		msg = std::format("radius = {:d}", radius);
		putText(drawing, msg, Point(ptTxt.x, ptTxt.y + 30 * 3), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 8);

		//avg_brightness - c code
		uchar* pData = src_color.data;
		uchar* pDataMask = src_hsv_bin.data;
		//only red channel
		int sum_red = 0;
		int sum_red_count = 0;
		for (size_t row = 0; row < src_color.rows; row++)
		{
			for (size_t col = 0; col < src_color.cols; col++)
			{
				int index = row * src_color.cols + col;
				int index_bgr = index*3;
				int red = pData[index_bgr + 2];
				int mask = pDataMask[index];
				if (red > 0 && mask == 255)
				{
					sum_red += red;
					sum_red_count++;
				}
			}
		}
		float avg_brightness_red = sum_red / sum_red_count;
		msg = std::format("avg_red = {:.1f}", avg_brightness_red);
		putText(drawing, msg, Point(ptTxt.x, ptTxt.y + 30 * 4), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 8);

		const int ptSz = 4;
		Point2f pt[ptSz];
		rrt.points(pt);
		for (size_t i = 0; i < ptSz; i++)
		{
			cv::line(drawing, pt[i % ptSz], pt[(i + 1) % ptSz], CV_RGB(255, 0, 255), 1);
		}
	}

	return 1;
}
