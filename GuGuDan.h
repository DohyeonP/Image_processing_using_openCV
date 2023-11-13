#pragma once

#include <string>
#include <vector>

class GuGuDan
{
public:
	GuGuDan();
	~GuGuDan();
	void SetParam(size_t dan_start = 0, size_t dan_end = 0, size_t step_start = 0, size_t step_end = 0);
	void proc();
	void display();

private:
	int dan_start, dan_end;
	int step_start, step_end;
	std::vector<std::string> vMsg;
};
