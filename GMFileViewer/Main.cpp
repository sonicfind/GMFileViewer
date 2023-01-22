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

		while (true)
		{
			std::cout << std::endl;
			file.displayModelList();
			std::cout << "Type the index for the model that you wish display [Or '0' to quit]\n";
			std::cout << "Note: any non-numerical response will also quit\n";
			uint32_t index;
			while (true)
			{
				std::cout << "Input: ";
				std::cin >> index;

				if (index <= file.getNumModels())
					break;

				std::cout << "Error: Index out of range\n";
			}
			
			if (index == 0)
				break;

			Graphics::initGraphics(Graphics::Backend::OpenGL);
			file.testGraphics(index - 1);
			Graphics::closeGraphics();
		}

		std::cout << "Press Enter to Exit" << std::endl;
		std::getline(std::cin, filename);
	}
	else if (path.extension() == U".CHC")
	{
		CHC file(path);
		std::cout << "Press Enter to Exit" << std::endl;
		std::getline(std::cin, filename);
	}
	else if (path.extension() == U".SSQ")
	{
		SSQ file(path);
		std::cout << "Press Enter to Exit" << std::endl;
		std::getline(std::cin, filename);
	}
	else if (path.extension() == U".WEB")
	{
		WEB file(path);
		std::cout << "Press Enter to Exit" << std::endl;
		std::getline(std::cin, filename);
	}
	else
	{
		std::cout << "Filetype not yet supported\n";
		std::cout << "Press Enter to Exit" << std::endl;
		std::getline(std::cin, filename);
	}
	
	return 0;
}
