#pragma once

#include "ISP.h"

int main()
{
	std::string fileName = "../KCCImageNet/images/ampoules/ampoules_02.png";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	size_t rows = src.rows;
	size_t cols = src.cols;

	cv::Mat src_bin;
	cv::threshold(src, src_bin, 205, 245, ThresholdTypes::THRESH_BINARY);

	cv::Mat src_color;
	cvtColor(src, src_color, COLOR_GRAY2BGR);

	//opening
	int type = MorphTypes::MORPH_OPEN;
	int shape = MorphShapes::MORPH_RECT;
	int kernelSz = 5;
	cv::Size sz = Size(2 * kernelSz + 1, 4 * kernelSz + 1);
	Mat SE = cv::getStructuringElement(shape, sz);
	cv::morphologyEx(src_bin, src_bin, type, SE);
	//end opening
	
	// find contours
	RNG rng(12345);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(src_bin, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	int cur_x_coordinate = 0, pre_x_coordinate = 0;

	std::vector<int> x_coordinate;
	Mat drawing = src_color.clone();

	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours, (int)i, color, CV_FILLED);

		RotatedRect rrt = minAreaRect(contours[i]);
		cv::drawMarker(drawing, rrt.center, CV_RGB(255, 0, 0));

		x_coordinate.push_back(rrt.center.x);
	}

	Mat vertical_cutting = src.clone();
	Mat horizontal_cutting = src.clone();

	uchar* pData_x = vertical_cutting.data;
	uchar* pData_y = horizontal_cutting.data;

	int threshold_y_min = 60;
	int threshold_y_max = 215;

	for (size_t index = 0; index < x_coordinate.size(); index++)
	{
		int x_negative = x_coordinate.at(index) - 10;
		if (x_negative < 0) x_negative = 0;
		
		int x_positive = x_coordinate.at(index) + 10;
		if (x_positive > vertical_cutting.cols)
			x_positive = vertical_cutting.cols - 1;

		for (size_t row = 0; row < rows; row++)
		{
			for (size_t col = 0; col < cols; col++)
			{
				int index = row * cols + col;
				if (x_negative <= col && col <= x_positive)
					pData_x[index] = 0;

				if (row < threshold_y_min || threshold_y_max < row)
					pData_y[index] = 0;
			}
		}
	}

	std::sort(x_coordinate.begin(), x_coordinate.end());

	// 맨앞 잡티 삭제
	if (x_coordinate.at(0) < 21)
		x_coordinate.erase(x_coordinate.begin());

	// 맨뒤 잡티 삭제
	if (cols - x_coordinate.at(x_coordinate.size() - 1) < 20)
		x_coordinate.pop_back();

	Mat result = vertical_cutting & horizontal_cutting;

	Mat result_Eq = result.clone();
	uchar* pData_result = result_Eq.data;

	cv::equalizeHist(result, result_Eq);

	// projection (direction : left ==> y - axis)
	Mat projection_result = Mat::zeros(src.size(), CV_8UC1);
	uchar* pData_pr = projection_result.data;
	int x = 0;
	int prev_x_max = 0;
	for (size_t row = 0; row < rows; row++)
	{
		int sum = 0;
		int count = 0;

		for (size_t col = 0; col < cols; col++)
		{
			int index = row * cols + col;

			if (threshold_y_min <= row && row <= threshold_y_max)
			{
				if (col == x_coordinate.at(x) || col == cols - 1)
				{
					if (count != 0)
						pData_pr[index - count - 10] = static_cast<uchar>(sum / count);
					prev_x_max = x_coordinate.at(x) + 10 + 1;
					if (x < x_coordinate.size() - 1) x++;
					count = 0;
					sum = 0;

					if (col == cols - 1)
					{
						x = 0;
						prev_x_max = 0;
					}

					continue;
				}
				
				int x_min = 0;
				int x_max = 0;

				if (x < x_coordinate.size())
				{
					x_min = x_coordinate.at(x) - 10;
					x_max = x_coordinate.at(x) + 10;
				}
				else
				{
					x_min = cols;
					x_max = cols;
				}

				if ((!(x_min <= col && col <= x_max) && prev_x_max <= col))
				{
					sum += pData_result[index];
					count++;
				}
			}
		}
	}

	uchar* pData_proj = projection_result.data;
	int index_vertical = 0;
	int next_index = 0;
	int current_index = 0;
	std::vector<Point> diff_index;
	for (size_t col = 0; col < cols; col++)
	{
		int diff = 0;
		int max_diff = diff;
		for (size_t row = 0; row < rows; row++)
		{
			// vertical index
			index_vertical = row * cols + col;
			next_index = (row + 1) * cols + col;
			if (pData_proj[index_vertical] != 0)
			{
				diff = pData_proj[next_index] - pData_proj[index_vertical];
				if (diff > max_diff)
				{
					max_diff = diff;
					current_index = index_vertical;
				}
			}
		}
		if (max_diff != 0) {
			if (col + 20 < cols)
			{
				diff_index.push_back(
					Point(col + 20, current_index / cols));
			}
			else
			{
				diff_index.push_back(
					Point(col, current_index / cols));
			}
		}
	}

	cv::Mat final_result = src_color.clone();

	// 상한 허용 위치
	int threshold_y_upper = 90;
	// 하한 허용 위치
	int threshold_y_lower = 150;

	cv::line(final_result, Point(0, threshold_y_upper), Point(cols - 1, threshold_y_upper), CV_RGB(255, 0, 0), 1);
	cv::line(final_result, Point(0, threshold_y_lower), Point(cols - 1, threshold_y_lower), CV_RGB(255, 0, 0), 1);

	// y 인덱스 값이 그 사이에 존재해야 한다.
	for (size_t i = 0; i < diff_index.size(); i++)
	{
		// 허용 위치에 있으면
		if (threshold_y_upper <= diff_index.at(i).y &&
			diff_index.at(i).y <= threshold_y_lower)
		{
			// 원 그리기
			cv::circle(final_result, diff_index.at(i), 20, CV_RGB(10, 50, 255));
		}
		// 허용 위치가 아니면
		else
		{
			// x 그리기
			cv::drawMarker(final_result, diff_index.at(i), CV_RGB(255, 0, 0), MarkerTypes::MARKER_TILTED_CROSS);
		}
	}

	return 1;
}