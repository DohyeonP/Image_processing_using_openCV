#pragma once

#include"ISP.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	cv::Mat src_gray_gaussian = src_gray.clone();
	cv::Mat src_gray_blur = src_gray.clone();

	const int filter_sz = 9;

	float blur[filter_sz * filter_sz] =	{ 0, };

	int gaussian_index[filter_sz] = {-4, -3, -2, -1, 0, 1, 2, 3, 4};

	double gaussian[filter_sz * filter_sz] = { 0, };

	float sig = 3;
	float PI = 3.141592;

	for (size_t y = 0; y < filter_sz; y++)
	{
		for (size_t x = 0; x < filter_sz; x++)
		{
            int index = y * filter_sz + x;
			float exp_ = std::exp(-1.0 *
				(gaussian_index[x] * gaussian_index[x] + gaussian_index[y] * gaussian_index[y])
				/ (2 * sig * sig));
            gaussian[index] = 1.0 / (2 * PI * sig * sig) * exp_;
		}
	}

	for (size_t index = 0; index < filter_sz * filter_sz; index++) {
		blur[index] = 1.0 / (filter_sz * filter_sz);
	}

	int half_kernelSize = filter_sz / 2;

	for (size_t row = half_kernelSize; row < src_gray.rows - half_kernelSize; row++)
	{
		for (size_t col = half_kernelSize; col < src_gray.cols - half_kernelSize; col++)
		{
            double gausian_sum = 0, blur_sum = 0;
			int count = 0;
			for (int f_row = -half_kernelSize; f_row <= half_kernelSize; f_row++)
			{
				for (int f_col = -half_kernelSize; f_col <= half_kernelSize; f_col++)
				{
					int index = (row + f_row) * src_gray.cols + (col + f_col);
					int f_index = (f_row + half_kernelSize) * filter_sz + (f_col + half_kernelSize);
					
					gausian_sum += src_gray_gaussian.data[index] * gaussian[f_index];
					blur_sum += src_gray_blur.data[index] * blur[f_index];
					count++;
				}
			}
			int index = (row) * src_gray.cols + (col);
			if (gausian_sum > 255) gausian_sum = 255;
			src_gray_gaussian.data[index] = static_cast<uchar>(gausian_sum);
			src_gray_blur.data[index] = static_cast<uchar>(blur_sum);
		}
	}

	return 1;
}