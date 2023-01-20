#include <iostream>
#include "FileTypes/XGM/XGM.h"
#include "FileTypes/CHC/CHC.h"
#include "FileTypes/SSQ/SSQ.h"
#include "FileTypes/WEB/WEB.h"
#include "Graphics.h"


int main()
{
	std::string filename;
	std::getline(std::cin, filename);

	if (filename[0] == '\"')
		filename = filename.substr(1, filename.length() - 2);

	std::filesystem::path path(filename);
	if (path.extension() == U".XGM")
	{
		XGM file(path);

		Graphics::initGraphics(Graphics::Backend::OpenGL);
		file.createGraphicsBuffers();
		file.testGraphics();
		Graphics::closeGraphics();

		std::cin.clear();
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
	else if (path.extension() == U".WEB")
	{
		WEB file(path);
		std::getline(std::cin, filename);
	}
	
	return 0;
}
