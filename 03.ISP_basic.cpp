#include "Common.h"
//#include <map>
#include <algorithm>

int main()
{
	//histogram 을 구하시오
	// 0 | ========== 10
	// 1 | ===== 5
	// 2 | ======== 8
	// ...
	// 255 | ============= 13

#if 0 // color
	std::string fileName = "../KCCImageNet/stop_img.png";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);

	uchar* pData = src.data;
	int length = src.total();

	int histogram[3][256] = {{0, 0},};

	int channel = src.channels();

	length *= channel;

	for (int index = 0; index < length; index++)
	{
		if (index % 3 == 2) // red
		{
			histogram[2][*(pData + index)]++;
		}
		if (index % 3 == 1) // green
		{
			histogram[1][*(pData + index)]++;
		}
		if (index % 3 == 0) // blue
		{
			histogram[0][*(pData + index)]++;
		}
	}

	for (int color = 0; color < 3; color++)
	{
		if (color == 0) std::cout << "blue\n" << std::endl;
		if (color == 1) std::cout << "green\n" << std::endl;
		if (color == 2) std::cout << "red\n" << std::endl;

		for (int index = 0; index < 256; index++)
		{
			std::cout << index << " | ";
			for (int data = 0; data < histogram[color][index]; data++)
			{
				std::cout << "=";
			}
			std::cout << " " << histogram[color][index] << std::endl;
		}
	}
#endif 

#if 1 // black & white
	std::string fileName = "../KCCImageNet/stinkbug.png";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	uchar* pData = src.data;
	int length = src.total();

	int histogram[256] = { 0, };

	for (int index = 0; index < length; index++)
	{
		histogram[*(pData + index)]++;
	}
	std::string strLine;
	for (int index = 0; index < 256; index++)
	{
		strLine = "";
		strLine += to_string(index);
		strLine += " | ";
		for (int data = 0; data < histogram[index]; data++)
		{
			strLine += "=";
		}
		strLine += to_string(histogram[index]);
		std::cout << strLine << std::endl;
	}
	auto _minmax = std::minmax_element(histogram, histogram + 256);
	int min_value = *_minmax.first;
	int max_value = *_minmax.second;

	std::cout << "최소값 : " << min_value << "\t최대값 : " << max_value << std::endl;

	cv::Mat draw = src.clone();
	cvtColor(draw, draw, ColorConversionCodes::COLOR_GRAY2BGR);
	for (size_t row = 0; row < 256; row++)
	{
		int scaled_cols = (histogram[row] * 1.0 / max_value) * 100;
		cv::Point st = Point(0, row);
		cv::Point ed = Point(scaled_cols, row);
		cv::line(draw, st, ed, CV_RGB(255, 255, 0));
	}

#endif

#if 0
	int datas[3 * 3] = { 5, 3, 1, 4, 6, 4, 8, 4, 3 };
	int count[10] = { 0, };
	int min_count = 10, max_count = -1;

	/*
	std::map<int, int> mData;

	// map initialization
	for (int data : datas)
	{
		mData.insert(pair<int, int>(data, 0));
	}

	// insert data
	for (int data : datas)
	{
		mData.insert(pair<int, int>(data, mData.at(data) + 1));
	}

	for (pair<int,int> p : mData)
	{
		//가장 높은 빈도 수
		if (p.second >= temp)
		{
			
		}

		//가장 낮은 빈도 수
	}
	*/

	for (int data : datas)
	{
		count[data]++;
		if (count[data] >= max_count) max_count = data;
		if (min_count <= count[data]) min_count = data;
	}

	for (int index = 0; index < sizeof(count) / sizeof(count[0]); index++)
	{
		if (count[index] == max_count)
		{
			std::cout << "가장 많은 빈도 값 : " << max_count << std::endl;
		}
		if (count[index] == min_count)
		{
			std::cout << "가장 적은 빈도 값 : " << min_count << std::endl;
		}
	}
#endif
	return 1;
}
