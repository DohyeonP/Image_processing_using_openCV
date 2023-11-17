#pragma once

#include "ISP.h"

int main()
{
	const int data_w = 7;
	const int data_h = 7;
	//uchar Datas[data_w * data_h] =
	//{
	//	1,1,0,1,1,
	//	1,1,0,1,1,
	//	0,0,0,0,0,
	//	1,1,0,1,1,
	//	1,1,0,1,1
	//};

	uchar Datas[data_w * data_h] =
	{
		000, 000, 000, 000, 000, 000, 000,
		000, 255, 255, 255, 000, 000, 000,
		000, 255, 255, 255, 255, 255, 000,
		000, 255, 000, 255, 255, 255, 000,
		000, 255, 255, 255, 255, 000, 000,
		000, 255, 255, 255, 000, 000, 000,
		000, 000, 000, 000, 000, 000, 000
	};

	//uchar Datas[data_w * data_h] =
	//{
	//	0,0,1,0,0,
	//	0,0,1,0,0,
	//	1,1,1,1,1,
	//	0,0,1,0,0,
	//	0,0,1,0,0
	//};

	Mat img_Datas = Mat(data_h, data_w, CV_8UC1, Datas);
	Mat img_Datas_erosion = Mat::zeros(data_h, data_w, CV_8UC1);

	// Ä§½Ä(erosion)
	const int SE_sz = 3;
	const int SE_half_sz = SE_sz / 2;
	int SE_Cross[SE_sz * SE_sz] =
	{
		0, 255, 0,
		255, 255, 255,
		0, 255, 0
	};
	int SE_Rect[SE_sz * SE_sz] =
	{
		255, 255, 255,
		255, 255, 255,
		255, 255, 255
	};

	// full image searching
	uchar* pDst = img_Datas_erosion.data;
	for (size_t row = SE_half_sz; row < data_h - SE_half_sz; row++)
	{
		for (size_t col = SE_half_sz; col < data_w - SE_half_sz; col++)
		{
			// struct element
			bool allone = true;
			for (int h = -SE_half_sz; h <= SE_half_sz; h++)
			{
				for (int w = -SE_half_sz; w <= SE_half_sz; w++)
				{
					int index = (row+h) * data_w + (col+w);
					int index_SE = (h + SE_half_sz)*SE_sz + (w + SE_half_sz);
					uchar input = Datas[index];
					// input == 0 : background
					bool condition = (input == 0 && SE_Rect[index_SE] == 255);
					if (condition == true)
					{
						allone = false;
						break;
					}
				}
				if (!allone) break;
			}
			if (allone)
				pDst[row * data_w + col] = 255;
			else
				pDst[row * data_w + col] = 0;
		}
	}

	return 1;
}