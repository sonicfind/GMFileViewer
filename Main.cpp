#include "FileTypes/XGM/XGM.h"
#include "FileTypes/CHC/CHC.h"
#include "FileTypes/SSQ/SSQ.h"
#include "TaskQueue.h"
#include <iostream>

int main()
{
	TaskQueue::getInstance();
	std::string filename;
	std::getline(std::cin, filename);

	if (filename[0] == '\"')
		filename = filename.substr(1, filename.length() - 2);

	std::filesystem::path path(filename);
	if (path.extension() == U".XGM")
	{
		XGM file(path);
		std::getline(std::cin, filename);
	}
	else if (path.extension() == U".CHC")
	{
		CHC file(path);
		std::getline(std::cin, filename);
	}
	else if (path.extension() == U".SSQ")
	{
		SSQ file(path);
		std::getline(std::cin, filename);
	}
	
	return 0;
}
