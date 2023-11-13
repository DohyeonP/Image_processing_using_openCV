#include "Common.h"


typedef struct {
	double h; // Hue (색조) [0, 360]
	double s; // Saturation (채도) [0, 1]
	double v; // Value (명도) [0, 1]
} HSV;

// RGB를 HSV로 변환
HSV RGBtoHSV(uint8_t r, uint8_t g, uint8_t b) {
	HSV hsv;
	double min, max, delta;
	r /= 255.0;
	g /= 255.0;
	b /= 255.0;
	min = fmin(fmin(r, g), b);
	max = fmax(fmax(r, g), b);
	hsv.v = max;
	delta = max - min;
	if (max > 0.0) { hsv.s = delta / max; }
	else {
		hsv.s = 0.0;
		hsv.h = 0.0;
		return hsv;
	}
	if (r == max) { hsv.h = (g - b) / delta; }
	else if (g == max) {
		hsv.h = 2.0 + (b - r) / delta;
	}
	else {
		hsv.h = 4.0 + (r - g) / delta;
	}
	hsv.h *= 60.0;
	if (hsv.h < 0.0) {
		hsv.h += 360.0;
	}
	return hsv;
}

int main()
{
#if 0
	std::string fileName = "../KCCImageNet/find_google_area.png";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);

	uchar* pData = src.data;
	int length = src.total();
	int channels = src.channels();

	cv::Mat src_Y = cv::Mat(src.rows, src.cols, CV_8UC1);
	cv::Mat src_Cb = cv::Mat(src.rows, src.cols, CV_8UC1);
	cv::Mat src_Cr = cv::Mat(src.rows, src.cols, CV_8UC1);
	cv::Mat src_RGB_new = cv::Mat(src.rows, src.cols, CV_8UC3);
	cv::Mat src_diff = cv::Mat(src.rows, src.cols, CV_8UC3);

	//
	for (size_t row = 0; row < src.rows; row++)
	{
		for (size_t col = 0; col < src.cols; col++)
		{
			// mono
			int index = (row)*src.cols + (col);

			// color
			if (channels == 3)
			{
				int index_B = index * channels + 0; // Blue
				int index_G = index * channels + 1; // Red
				int index_R = index * channels + 2; // Green
				int val_B = pData[index_B];
				int val_G = pData[index_G];
				int val_R = pData[index_R];

				// Y
				double val_Y = 0.299 * val_R + 0.587 * val_G + 0.114 * val_B;
				src_Y.data[index] = (uchar)val_Y;

				// Cb
				double val_Cb = -0.169 * val_R - 0.331 * val_G + 0.500 * val_B;
				src_Cb.data[index] = (uchar)val_Cb;

				// Cr
				double val_Cr = 0.500 * val_R - 0.419 * val_G - 0.0813 * val_B;
				src_Cr.data[index] = (uchar)val_Cr;

				// YCbCr -> RGB
				double RGB_R = 1.000 * val_Y + 1.402 * val_Cr + 0.000 * val_Cb;
				double RGB_G = 1.000 * val_Y - 0.714 * val_Cr - 0.344 * val_Cb;
				double RGB_B = 1.000 * val_Y + 0.000 * val_Cr + 1.772 * val_Cb;

				src_RGB_new.data[index * channels + 0] = (uchar)RGB_B;
				src_RGB_new.data[index * channels + 1] = (uchar)RGB_G;
				src_RGB_new.data[index * channels + 2] = (uchar)RGB_R;
			}
			// mono
			else
			{
				src_Y.data[index] = src.data[index];
			}
		}
	}
	src_diff = src - src_RGB_new;
#else
	std::string fileName = "../KCCImageNet/find_google_area.png";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);
	cv::Mat src_HSV = cv::Mat(src.rows, src.cols, COLOR_BGR2HSV);

	uchar* pData = src.data;
	int length = src.total();
	int channels = src.channels();

	for (size_t row = 0; row < src.rows; row++)
	{
		for (size_t col = 0; col < src.cols; col++)
		{
			// mono
			int index = (row)*src.cols + (col);

			if (channels == 3)
			{
				int index_B = index * channels + 0; // Blue
				int index_G = index * channels + 1; // Red
				int index_R = index * channels + 2; // Green
				int val_B = pData[index_B];
				int val_G = pData[index_G];
				int val_R = pData[index_R];
				
				HSV hsv = RGBtoHSV(val_R, val_G, val_B);

				src_HSV.data[index_B] = (uchar)hsv.s;
				src_HSV.data[index_G] = (uchar)hsv.v;
				src_HSV.data[index_R] = (uchar)hsv.h;
			}
		}
	}

#endif
	return 1;
}