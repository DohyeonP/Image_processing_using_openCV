#pragma once

#include"ISP.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena_gray.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	cv::Mat src_restore = src.clone();

	int restore = 0;
	int restore_count = 0;

	for (size_t row = 0; row < src_restore.rows; row++)
	{
		for (size_t col = 0; col < src_restore.cols; col++)
		{
            int index = row * src_restore.cols + col;

			// 데이터가 손상이 되었다면
			if (src_restore.data[index] >= 253 || src_restore.data[index] <= 1)
			{
				// 상
				if (index - src_restore.cols > 0)
				{
					restore += src_restore.data[index - src_restore.cols];
					restore_count++;
				}
			
				// 하
				if (index + src_restore.cols < src_restore.rows * src_restore.cols)
				{
					restore += src_restore.data[index + src_restore.cols];
					restore_count++;
				}

				// 좌
				if ((index - 1) % src_restore.cols != src_restore.cols - 1)
				{
					restore += src_restore.data[index - 1];
					restore_count++;
				}

				// 우
				if (index + 1 % src_restore.cols != 0)
				{
					restore += src_restore.data[index + 1];
					restore_count++;
				}

				// 대각 좌상
				if (index - src_restore.cols > 0 &&
					(index - 1) % src_restore.cols != src_restore.cols - 1)
				{
					restore += src_restore.data[index - src_restore.cols - 1];
					restore_count++;
				}

				// 대각 우상
				if (index - src_restore.cols > 0 && index + 1 % src_restore.cols != 0)
				{
					restore += src_restore.data[index - src_restore.cols + 1];
					restore_count++;
				}

				// 대각 좌하
				if (index + src_restore.cols < src_restore.rows * src_restore.cols &&
					(index - 1) % src_restore.cols != src_restore.cols - 1)
				{
					restore += src_restore.data[index + src_restore.cols - 1];
					restore_count++;
				}

				// 대각 우하
				if (index + src_restore.cols < src_restore.rows * src_restore.cols &&
					index + 1 % src_restore.cols != 0)
				{
					restore += src_restore.data[index + src_restore.cols + 1];
					restore_count++;
				}

				if (restore_count != 0)
				{
					restore = restore / restore_count;
					src_restore.data[index] = restore;
					restore_count = 0;
				}
			}
		}
    }

	cv::Mat diff = src - src_restore;

	return 1;
}