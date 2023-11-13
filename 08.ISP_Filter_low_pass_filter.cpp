#pragma once

#include"ISP.h"

// ppt 5장 p.11 저역 통과 필터
int main()
{
#if 1
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	cv::Mat src_gray_blur = src_gray.clone();
	cv::Mat src_gray_blur_1 = src_gray.clone();
	cv::Mat src_gray_blur_2 = src_gray.clone();

	src_gray_blur = 0;
    src_gray_blur_1 = 0;
	src_gray_blur_2 = 0;

	const int filter_sz = 3;

	float blur[] = {
		1.0 / 9, 1.0 / 9, 1.0 / 9,
		1.0 / 9, 1.0 / 9, 1.0 / 9,
		1.0 / 9, 1.0 / 9, 1.0 / 9
	};

	float blur_1[] = {
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	};

	float blur_2[] = {
		2.0 / 25, 3.0 / 25, 2.0 / 25,
		3.0 / 25, 5.0 / 25, 3.0 / 25,
		2.0 / 25, 3.0 / 25, 2.0 / 25
	};

	int half_kernelSize = filter_sz / 2;

	for (size_t row = half_kernelSize; row < src_gray.rows - half_kernelSize; row++)
	{
		for (size_t col = half_kernelSize; col < src_gray.cols - half_kernelSize; col++)
		{
			int sum = 0, sum1 = 0, sum2 = 0;
			for (int f_row = -half_kernelSize; f_row <= half_kernelSize; f_row++)
			{
				for (int f_col = -half_kernelSize; f_col <= half_kernelSize; f_col++)
				{
					int index = (row + f_row) * src_gray.cols + (col + f_col);
					int f_index = (f_row + half_kernelSize) * filter_sz + (f_col + half_kernelSize);
					sum += src_gray.data[index] * blur[f_index];
					sum1 += src_gray.data[index] * blur_1[f_index];
					sum2 += src_gray.data[index] * blur_2[f_index];
				}
			}
			int index = (row) * src_gray.cols + (col);
			src_gray_blur.data[index] = static_cast<uchar>(sum);
			src_gray_blur_1.data[index] = static_cast<uchar>(sum1);
			src_gray_blur_2.data[index] = static_cast<uchar>(sum2);
		}
	}

	// 모서리가 아닌 중간 부분
	// row
    for (size_t row = 1; row < src_gray_blur.rows - 1; row++) {
		// 왼쪽
		int index_0 = row * src_gray_blur.cols + 0;
        int index_1 = row * src_gray_blur.cols + 1;
        src_gray_blur.data[index_0] = src_gray_blur.data[index_1];
        src_gray_blur_1.data[index_0] = src_gray_blur_1.data[index_1];
        src_gray_blur_2.data[index_0] = src_gray_blur_2.data[index_1];

		// 오른쪽
        int index_512 = row * src_gray_blur.cols + src_gray_blur.cols - 1;
        int index_511 = row * src_gray_blur.cols + src_gray_blur.cols - 2;
        src_gray_blur.data[index_512] = src_gray_blur.data[index_511];
        src_gray_blur_1.data[index_512] = src_gray_blur_1.data[index_511];
        src_gray_blur_2.data[index_512] = src_gray_blur_2.data[index_511];
    }

	// col
	for (size_t col = 1; col < src_gray_blur.cols - 1; col++) {
		// 위
        int index_0 = col;
        int index_1 = col + src_gray_blur.rows;
        src_gray_blur.data[index_0] = src_gray_blur.data[index_1];
        src_gray_blur_1.data[index_0] = src_gray_blur_1.data[index_1];
        src_gray_blur_2.data[index_0] = src_gray_blur_2.data[index_1];

		// 아래
        int index_512 = col + ((src_gray_blur.rows - 1) * src_gray_blur.cols);
        int index_511 = col + ((src_gray_blur.rows - 2) * src_gray_blur.cols);
        src_gray_blur.data[index_512] = src_gray_blur.data[index_511];
        src_gray_blur_1.data[index_512] = src_gray_blur_1.data[index_511];
        src_gray_blur_2.data[index_512] = src_gray_blur_2.data[index_511];
    }

	// indexing = (row) * cols + (col)

	// 모서리
	// 
	// 왼쪽 위
    src_gray_blur.data[0] = (src_gray_blur.data[1] + src_gray_blur.data[src_gray_blur.rows]) / 2;
	src_gray_blur_1.data[0] = (src_gray_blur_1.data[1] + src_gray_blur_1.data[src_gray_blur_1.rows]) / 2;
	src_gray_blur_2.data[0] = (src_gray_blur_2.data[1] + src_gray_blur_2.data[src_gray_blur_2.rows]) / 2;
    /*
	src_gray_blur.data[0] = (src_gray_blur.data[] + src_gray_blur.data[src_gray_blur.rows]) / 2
	*/
	
	// 오른쪽 위
    src_gray_blur.data[src_gray_blur.cols - 1] =
		(src_gray_blur.data[src_gray_blur.cols - 2] +
		 src_gray_blur.data[src_gray_blur.cols + src_gray_blur.rows - 1]) / 2;

    src_gray_blur_1.data[src_gray_blur_1.cols - 1] =
        (src_gray_blur_1.data[src_gray_blur_1.cols - 2] +
         src_gray_blur_1.data[src_gray_blur_1.cols + src_gray_blur_1.rows - 1]) / 2;

    src_gray_blur_2.data[src_gray_blur_2.cols - 1] =
        (src_gray_blur_2.data[src_gray_blur_2.cols - 2] +
         src_gray_blur_2.data[src_gray_blur_2.cols + src_gray_blur_2.rows - 1]) / 2;
    /*
	src_gray_blur.data[src_gray_blur.data[cols-1]] = (src_gray_blur.data[cols-2] + src_gray_blur.data[src_gray_blur.cols + src_gray_blur.rows - 1]) / 2
	*/

	// 왼쪽 아래
    src_gray_blur.data[(src_gray_blur.rows - 1) * src_gray_blur.cols] =
        (src_gray_blur.data[(src_gray_blur.rows - 2) * src_gray_blur.cols] +
         src_gray_blur.data[(src_gray_blur.rows - 1) * src_gray_blur.cols + 1]) / 2;

    src_gray_blur_1.data[(src_gray_blur_1.rows - 1) * src_gray_blur_1.cols] =
        (src_gray_blur_1.data[(src_gray_blur_1.rows - 2) * src_gray_blur_1.cols] +
         src_gray_blur_1.data[(src_gray_blur_1.rows - 1) * src_gray_blur_1.cols + 1]) / 2;

	src_gray_blur_2.data[(src_gray_blur_2.rows - 1) * src_gray_blur_2.cols] =
        (src_gray_blur_2.data[(src_gray_blur_2.rows - 2) * src_gray_blur_2.cols] +
         src_gray_blur_2.data[(src_gray_blur_2.rows - 1) * src_gray_blur_2.cols + 1]) / 2;

	// 오른쪽 아래
    src_gray_blur.data[src_gray_blur.rows * src_gray_blur.cols - 1] =
        (src_gray_blur.data[(src_gray_blur.rows -1) * src_gray_blur.cols - 1] +
         src_gray_blur.data[src_gray_blur.rows * src_gray_blur.cols - 2]) / 2;

	src_gray_blur_1.data[src_gray_blur_1.rows * src_gray_blur_1.cols - 1] =
        (src_gray_blur_1.data[(src_gray_blur_1.rows -1) * src_gray_blur_1.cols - 1] +
         src_gray_blur_1.data[src_gray_blur_1.rows * src_gray_blur_1.cols - 2]) / 2;

	src_gray_blur_2.data[src_gray_blur_2.rows * src_gray_blur_2.cols - 1] =
        (src_gray_blur_2.data[(src_gray_blur_2.rows -1) * src_gray_blur_2.cols - 1] +
         src_gray_blur_2.data[src_gray_blur_2.rows * src_gray_blur_2.cols - 2]) / 2;

#else
	int datas[] = { 6,4,8,9,4,4,8,64,4,6,4,8,6,4,11,1,3,1134,5,64,5,64 };
	
	// 노이즈가 없는 대표 신호 값을 구하시오.
	
	// 노이즈 제거를 위해 최소값, 최대값 삭제 혹은
	// 중간값의 특정한 바운더리를 설정

	std::sort(datas, datas + std::size(datas));

	int filter_datas[15];
	int index = 0;

	std::cout << "원본 데이터" << std::endl;

	for (int data : datas)
	{
		std::cout << data << "  ";
	}

	std::cout << std::endl;

	std::cout << "\n걸러진 데이터" << std::endl;

	for (int data : datas)
	{
		if (data > 3 && data < 10)
		{
			std::cout << "data : " << data << std::endl;
			filter_datas[index] = data;
			index++;
		}
	}

	std::cout << "\n필터된 값의 중간값 : " << filter_datas[std::size(filter_datas) / 2] << std::endl;

	// 정규 분포
	double normal[std::size(datas)];
	index = 0;

	std::cout << "\n정규화된 값 : " << std::endl;

	for (int data : datas)
	{
		normal[index] = (1.0 * data) / (1.0 * datas[std::size(datas) - 1]);

		std::cout << normal[index] << "  ";

		index++;
	}

	std::cout << "\n\n정규화된 값의 중간 값" << std::endl;
	std::cout << normal[std::size(normal) / 2] << std::endl;

	std::cout << "\n정규화된 값의 중간값 복원" << std::endl;
	std::cout << normal[std::size(normal) / 2] * datas[std::size(datas) - 1] << std::endl;

	std::cout << "\n정규화된 값의 평균 값" << std::endl;
	double result = 0;
	for (double data : normal)
	{
		result += data;
	}
	std::cout << result / std::size(normal) << std::endl;
#endif
	return 1;
}