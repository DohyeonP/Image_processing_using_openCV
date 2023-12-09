#pragma once

#include "Common.h"

void MatchingMethod(const Mat& serch_img, const Mat& ptrn_img, const double& thres, vector<Point>& rois)
{
	Mat result = Mat::zeros(Size(serch_img.cols - ptrn_img.cols + 1, serch_img.rows - ptrn_img.rows + 1), CV_32FC1);
	int match_method = TM_CCORR_NORMED;
	matchTemplate(serch_img, ptrn_img, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());


	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	bool multiobjects = true;
	if (!multiobjects)
	{
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
		if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
		{
			matchLoc = minLoc;
		}
		else
		{
			matchLoc = maxLoc;
		}

		rois.push_back(Point(matchLoc.x, matchLoc.y));
	}
	else
	{
		Mat tm_bin;
		double min_thr = thres;
		double max_thr = 1.0;
		cv::threshold(result, tm_bin, min_thr, max_thr, ThresholdTypes::THRESH_BINARY);
		tm_bin *= 255;

		tm_bin.convertTo(tm_bin, CV_8UC1);

		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(tm_bin, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

		for (size_t i = 0; i < contours.size(); i++)
		{
			RotatedRect rrt = minAreaRect(contours[i]);
			//rois.push_back(rrt.boundingRect());
			rois.push_back(Point(rrt.boundingRect().tl().x, rrt.boundingRect().tl().y));
		}
	}
}

void main()
{
	// source
	std::string src_filename = "../KCCImageNet/solar_cell/solar_cell_";
	std::string src_number = std::format("0{:d}.png", 7);
	src_filename = src_filename + src_number;

	// color
	cv::Mat src_color = cv::imread(src_filename, cv::ImreadModes::IMREAD_COLOR);
	
	// channels
	std::vector<cv::Mat> channels;
	cv::split(src_color, channels);

//	cv::Mat B = channels[0];
//	cv::Mat G = channels[1];
	cv::Mat R = channels[2];

	int rows = R.rows;
	int cols = R.cols;

	cv::Mat R_binary;
	cv::threshold(R, R_binary, 127, 255, THRESH_BINARY_INV);

	// find contours
	RNG rng(12345);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<vector<Point>> hole;
	findContours(R_binary, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
	Mat drawing = src_color.clone();// Mat::zeros(src_color.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		//drawContours(drawing, contours, (int)i, color, 1, LINE_8, hierarchy, 0);
		drawContours(drawing, contours, (int)i, color, CV_FILLED);

		// x 그리기
		RotatedRect rrt = minAreaRect(contours[i]);
		cv::drawMarker(drawing, rrt.center, CV_RGB(255, 0, 0), MarkerTypes::MARKER_TILTED_CROSS, 10, 5);
		int a12 = 0;

		// size 가 985 이상 2000 이하면 구멍 의심
		if (985 < contours[i].size() && contours[i].size() < 2000)
		{
			hole.push_back(contours[i]);
		}
	}
	
	// 구멍 찾는 알고리즘
	uchar* p_bData = R_binary.data;
	for (size_t i = 0; i < hole.size() - 1; i++)
	{
		RotatedRect rrt = minAreaRect(hole[i]);
		for (size_t k = 1; k < hole[i].size() - 1; k++)
		{
			// 대각 방향으로 이동했을 때
			if (hole[i][k - 1].x - hole[i][k].x == std::abs(1) &&
				hole[i][k - 1].y - hole[i][k].y == std::abs(1))
			{
				int x_position = hole[i][k].x;
				int y_position = hole[i][k].y;
				int index = y_position * cols + x_position;

				// 구멍 찾기
				// 이 위치가 y 포지션과 차이가 거의 없어야 한다 (1정도의 차이)
				if (p_bData[index - cols] == 255 && p_bData[index + cols] == 255 &&
					std::abs((index - x_position) / cols - rrt.center.y) <= 1)
				{
					// 구멍이라고 판단 & 그림 그리기
					int radius = 10;
					cv::circle(drawing, Point(x_position, y_position), radius, CV_RGB(255, 0, 0));
				}
			}
		}
	}
	
	// 티끌 찾기
	std::string pattern_filename = "../KCCImageNet/solar_cell/pattern.png";
	cv::Mat pattern = cv::imread(pattern_filename, cv::ImreadModes::IMREAD_GRAYSCALE);

	cv::Mat result;

	double thres = 0.99;
	vector<Point> finds;
	MatchingMethod(R_binary, pattern, thres, finds);

	for (size_t k = 0; k < finds.size(); k++)
	{
		cv::circle(drawing, finds[k], 10, CV_RGB(255, 255, 0));
	}

	// debug point
	int a = 0;
}