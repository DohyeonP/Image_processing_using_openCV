#include <iostream>

#if 0
#include "GuGuDan.h"
int main()
{
	int dan_start, dan_end;
	int step_start, step_end;

	GuGuDan gugu;

	std::cout << "dan_start : ";
	std::cin >> dan_start;

	std::cout << "dan_end : ";
	std::cin >> dan_end;

	std::cout << "step_start : ";
	std::cin >> step_start;

	std::cout << "step_end : ";
	std::cin >> step_end;

	gugu.SetParam(dan_start, dan_end, step_start, step_end);
	gugu.proc();
	gugu.display();

	return 1;
}
#endif

#if 1
#include <opencv2/opencv.hpp>
#include <iostream>
int main()
{
	std::cout << "Hello OpenCV " << CV_VERSION << std::endl;

	return 0;
}
#endif