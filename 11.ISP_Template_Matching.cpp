#pragma once

#include "Common.h"

const cv::Mat& template_matching(const cv::Mat& src, const cv::Mat& pattern)
{
	cv::Mat result;



	return result;
}


void main()
{
	// source
	std::string src_filename = "../KCCImageNet/images/ampoules/ampoules_";
	std::string src_number = std::format("0{:d}.png", 2);
	src_filename = src_filename + src_number;
	cv::Mat src = cv::imread(src_filename, cv::ImreadModes::IMREAD_GRAYSCALE);

	// pattern
	std::string ptrn_filename = "../KCCImageNet/images/ampoules/pattern.tif";
	cv::Mat pattern = cv::imread(ptrn_filename, cv::ImreadModes::IMREAD_GRAYSCALE);

	// source rows & cols
	int s_rows = src.rows, s_cols = src.cols;

	// pattern rows & cols
	int p_rows = pattern.rows, p_cols = pattern.cols;

	// find region
	cv::Mat fine_resion = template_matching(src, pattern);

	// debug point
	int a = 0;
}



#if 0
void main()
{
	std::string inString = "ksdfer_kan_abcd_dqewabcdaf_abcdwqe_abcddasgv";
	std::string inPtrn = "abcd";

	int size = inString.size() - inPtrn.size();
	int knSize = inPtrn.size();
	std::vector<int> equal_index;

	for (size_t index = 0; index < size; index++)
	{
		int equal_count = 0;
		for (size_t k_index = 0; k_index < knSize; k_index++)
		{
			if (inString.at(k_index+index) - inPtrn.at(k_index) == 0)
				equal_count++;
		}

		if (equal_count == knSize)
		{
			equal_index.push_back(index);
		}
	}

	std::cout << "일치한 갯수 : " << equal_index.size() << std::endl;
	
	for (int index : equal_index)
	{
		std::cout << "인덱스 시작 위치 : " << index << std::endl;
	}
}
#endif