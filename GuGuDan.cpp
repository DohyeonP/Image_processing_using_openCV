#include <iostream>
#include "GuGuDan.h"

GuGuDan::GuGuDan()
	: dan_start(0)
	, dan_end(0)
	, step_start(0)
	, step_end(0)
{
	std::cout << "GuGuDan::Ctor" << std::endl;
}

GuGuDan::~GuGuDan()
{
	std::cout << "GuGuDan::Dtor" << std::endl;
}
void GuGuDan::SetParam(size_t dan_start, size_t dan_end, size_t step_start, size_t step_end)
{
	this->dan_start = dan_start; this->dan_end = dan_end;
	this->step_start = step_start; this->step_end = step_end;
}

void GuGuDan::proc()
{
	vMsg.clear();

	for (size_t step = step_start; step < step_end + 1; step++)
	{
		std::string msg = "";
		for (size_t dan = dan_start; dan < dan_end + 1; dan++)
		{
			msg += std::to_string(dan);
			msg += "*";
			msg += std::to_string(step);
			msg += "=";
			msg += std::to_string(dan * step);
			msg += "\t";
		}
		vMsg.push_back(msg);
	}
}

void GuGuDan::display()
{
	for (const auto& msg : vMsg)
	{
		std::cout << msg << std::endl;
	}
}