#pragma once

#include"ISP.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);
	
	cv::Mat src_gray_sobel = src_gray.clone();
	cv::Mat src_gray_sobel_x = src_gray.clone();
	cv::Mat src_gray_sobel_y = src_gray.clone();

	const int filter_sz = 3;

	int vertical_filter[] = {
		1, 0, -1,
		2, 0, -2,
		1, 0, -1
	};

	int horizontal_filter[] = {
		-1, -2, -1,
		0, 0, 0,
		1, 2, 1
	};

	int half_kernelSize = filter_sz / 2;
	int threshold = 80;

	for (size_t row = half_kernelSize; row < src_gray.rows - half_kernelSize; row++)
	{
		for (size_t col = half_kernelSize; col < src_gray.cols - half_kernelSize; col++)
		{
            int vertical_sum = 0, horizontal_sum = 0, count = 0;
			for (int f_row = -half_kernelSize; f_row <= half_kernelSize; f_row++)
			{
				for (int f_col = -half_kernelSize; f_col <= half_kernelSize; f_col++)
				{
					int index = (row + f_row) * src_gray.cols + (col + f_col);
					int f_index = (f_row + half_kernelSize) * filter_sz + (f_col + half_kernelSize);
					vertical_sum += src_gray.data[index] * vertical_filter[f_index];
					horizontal_sum += src_gray.data[index] * horizontal_filter[f_index];
					count++;
				}
			}
			int index = (row) * src_gray.cols + (col);

			src_gray_sobel_x.data[index] = std::abs(horizontal_sum);
			src_gray_sobel_y.data[index] = std::abs(vertical_sum);

			src_gray_sobel.data[index] = (unsigned char)sqrt((double)(horizontal_sum * horizontal_sum + vertical_sum * vertical_sum));
		}
	}

		//copy col+1 to col, col[1]->col[0], left
	for (size_t row = 0; row < src_gray_sobel.rows; row++)
	{
		int index_0 = row * src_gray_sobel.cols + 0;
		int index_1 = row * src_gray_sobel.cols + 1;
		src_gray_sobel.data[index_0] = src_gray_sobel.data[index_1];

		index_0 = row * src_gray_sobel.cols + (src_gray_sobel.cols - 1);
		index_1 = row * src_gray_sobel.cols + (src_gray_sobel.cols - 2);
		src_gray_sobel.data[index_0] = src_gray_sobel.data[index_1];
	}
	//copy col to col+1, col[98]->col[99], right
	for (size_t col = 0; col < src_gray_sobel.cols; col++)
	{
		int index_0 = (0) * src_gray_sobel.cols + col;
		int index_1 = (1) * src_gray_sobel.cols + col;
		src_gray_sobel.data[index_0] = src_gray_sobel.data[index_1];

		index_0 = (src_gray_sobel.rows - 1) * src_gray_sobel.cols + col;
		index_1 = (src_gray_sobel.rows - 2) * src_gray_sobel.cols + col;
		src_gray_sobel.data[index_0] = src_gray_sobel.data[index_1];
	}
	
	{
		Mat mag;
		Mat dx, dy;
		Sobel(src_gray, dx, CV_32FC1, 1, 0);
		Sobel(src_gray, dy, CV_32FC1, 0, 1);
		magnitude(dx, dy, mag);
		mag.convertTo(mag, CV_8UC1);
		Mat edge = mag >= threshold;
		int a = 0;
	}

	return 1;
}