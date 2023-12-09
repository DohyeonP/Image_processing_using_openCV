//============================================================
// 11/21 Q2 solar_cell      
//============================================================
#if 1

#include "Common.h"

void main()
{
	std::string fileName = "../KCCImageNet/solar_cell/solar_cell_03.1.png";
	cv::Mat src_draw = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	//object를 추출하기 위해 red 채널(src_r) 이용
	cv::Mat src_color = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);
	cv::Mat src_chanells[3];   //b-g-r;
	cv::split(src_color, src_chanells);
	cv::Mat src_r = src_chanells[2];
	cv::Mat src_g = src_chanells[1];
	cv::Mat src_b = src_chanells[0];


	uchar* pData = src_gray.data;
	size_t width = src_gray.cols;
	size_t height = src_gray.rows;

	//src_r을 이진화
	int threshold_min = 128;   //128
	int threshold_max = 255;   //255
	cv::Mat src_bin = Mat::zeros(cv::Size(width, height), CV_8UC1);
	uchar* pDataBin = src_bin.data;
	cv::threshold(src_r, src_bin, threshold_min, threshold_max, ThresholdTypes::THRESH_BINARY_INV);      //THRESH_BINARY보다 THRESH_BINARY_INV 이용하는 게 편함


	//morphology
	int kernelSz = 1;
	int shape = MorphShapes::MORPH_ELLIPSE;
	cv::Size sz = Size(2 * kernelSz + 1, 1 * kernelSz + 1);
	Mat SE = cv::getStructuringElement(shape, sz);
	Mat src_open;
	int type = MorphTypes::MORPH_OPEN;
	cv::morphologyEx(src_bin, src_open, type, SE);


	RNG rng(12345);
	vector<vector<Point>> contours;
	vector<Point> contours_center;
	vector<Vec4i> hierarchy;
	findContours(src_bin, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	Mat drawing = Mat::zeros(src_bin.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		//drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
		drawContours(drawing, contours, (int)i, color, CV_FILLED);
	}

	//면적으로 분류하는 방식 (작은사각형에서 끊김이 발생한 면적은 큰사각형 면적과 동일해서 구분이 어려움.....)
	double avg_area_LR = 4300;
	double avg_area_M = avg_area_LR * 2;   //작은사각형 2배한 면적이 큰사각형 면적과 비슷
	double per = 0.1;   //10%
	double spec_area_LR_min = avg_area_LR - avg_area_LR * per;
	double spec_area_LR_max = avg_area_LR + avg_area_LR * per;
	double spec_area_M_min = avg_area_M - avg_area_M * per;
	double spec_area_M_max = avg_area_M + avg_area_M * per;

	double spec_area_large_min = 100000;
	double spec_area_small_max = 100;

	//높이로 분류하는 방식
	double avg_height = 20;
	double spec_height_min = avg_height - avg_height * per;
	double spec_height_max = avg_height + avg_height * per;
	std::vector<cv::Rect> vAbnormalRegions;
	for (size_t i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		if (area > spec_area_large_min) continue;
		if (area < spec_area_small_max) continue;

		RotatedRect rrt = minAreaRect(contours[i]);
		if (rrt.boundingRect().height < 15) continue;
		double arcLen = arcLength(contours[i], true);

		int rgn_Height = rrt.boundingRect().height;
		//if (spec_area_LR_min <= area && area <= spec_area_LR_max)
		//{
		//   //Normal
		//}
		//else if (spec_area_M_min <= area && area <= spec_area_M_max)
		//{
		//   //Normal
		//}
		if (spec_height_min <= rgn_Height && rgn_Height <= spec_height_max)
		{
		}
		else
		{
			//Abnormal
			vAbnormalRegions.push_back(rrt.boundingRect());
		}

	}

	for (size_t i = 0; i < vAbnormalRegions.size(); i++)      //break point
	{
		cv::Rect region = vAbnormalRegions[i];
		int deflate = 2;
		region.x += deflate;
		region.y += deflate;
		region.width -= deflate * 2;
		region.height -= deflate * 2;
		cv::rectangle(src_draw, region, CV_RGB(255, 0, 0), 1);
		cv::Mat subRegion = src_r(region).clone();

		threshold(subRegion, subRegion, 100, 255, ThresholdTypes::THRESH_BINARY);
		//find out the subRegion's broken point 
		int detected_X_line = 0;
		vector<int> accumulated_vertical;
		for (int x = 0; x < region.width; x++)
		{
			int sum_line = 0;
			for (int y = 0; y < region.height; y++)
			{
				Point p = Point(x, y);
				sum_line += subRegion.at<uchar>(p.y, p.x);
			}
			accumulated_vertical.push_back(sum_line);
		}
		//int index_of_maxValue = max_element(grads.begin(), grads.end()) - grads.begin();
		//int index_of_minValue = min_element(accumulated_vertical.begin(), accumulated_vertical.end()) - accumulated_vertical.begin();

		int adaptThresAcc = 0;
		if (35 < region.height && region.height < 45)
			adaptThresAcc = 200;
		if (55 < region.height && region.height < 65)
			adaptThresAcc = 600;

		std::vector<int> vDetects;
		for (size_t i = 0; i < accumulated_vertical.size(); i++)//
		{
			if (accumulated_vertical[i] < adaptThresAcc)
			{
				vDetects.push_back(i);





				Rect rt(i + region.x, region.y, 1, region.height);
				cv::rectangle(src_draw, rt, Scalar(128, 255, 255, 80),1);
			}
		}

		continue;



		//1차 미분 값을 이용한 큰 변화랑 위치(index) 구하기
		vector<int> grads;
		for (size_t i = 1; i < accumulated_vertical.size(); i++)//
		{
			int pre = accumulated_vertical[i - 1];
			int cur = accumulated_vertical[i];
			//1차 미분
			int diff = std::abs(cur - pre);
			//int diff = (cur - pre);
			grads.push_back(diff);

		}

		//int index_of_maxValue = max_element(grads.begin(), grads.end()) - grads.begin();
		//int index_of_minValue = min_element(grads.begin(), grads.end()) - grads.begin();

		int max_index = 0;
		int max_value = 0;
		for (size_t i = 0; i < grads.size(); i++)//
		{
			if (max_value < grads[i])
			{
				max_value = grads[i];
				max_index = i;
			}
		}


		//full(global)  =  local   +  full(global)
		detected_X_line = max_index + region.x;

		Scalar judge_color(0, 255, 0);
		Scalar judge_color_OK(0, 255, 0);
		Scalar judge_color_NG(0, 0, 255);
		Point detected_pt1 = Point(detected_X_line, region.y);
		Point detected_pt2 = Point(detected_X_line, region.y + region.height);

		circle(src_draw, detected_pt1, 10, Scalar(255, 0, 255), 1, 8, 0);
		
		int a = 0;
	}


	int a = 0;
}

#endif