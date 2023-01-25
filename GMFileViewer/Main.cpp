#include <iostream>
#include "FileTypes/XGM/XGM.h"
#include "FileTypes/CHC/CHC.h"
#include "FileTypes/SSQ/SSQ.h"
#include "FileTypes/WEB/WEB.h"
#include "Graphics.h"

std::string g_filename;

template <class T>
void testWrite(const T& file)
{
	std::cout << "Test Write? [Y/N]\n";
	while (true)
	{
		char answer;
		std::cout << "Input: ";
		std::cin >> answer;
		if (answer == 'Y' || answer == 'y')
		{
			file.saveToFile(g_filename.insert(g_filename.length() - 4, "_test"));
			break;
		}
		else if (answer == 'N' || answer == 'n')
			break;
	}
	std::getline(std::cin, g_filename);
}

int main()
{
	std::getline(std::cin, g_filename);

	if (g_filename[0] == '\"')
		g_filename = g_filename.substr(1, g_filename.length() - 2);

	if (g_filename.ends_with(".XGM"))
	{
		XGM file(g_filename);

		while (true)
		{
			std::cout << std::endl;
			file.displayModelList();
			std::cout << "Type the index for the model that you wish display [Or '0' to quit]\n";
			std::cout << "Note: any non-numerical response will also quit\n";
			size_t index;
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

		testWrite(file);

		std::cout << "Press Enter to Exit" << std::endl;
		std::getline(std::cin, g_filename);
	}
	else if (g_filename.ends_with(".CHC"))
	{
		CHC file(g_filename);
		testWrite(file);

		std::cout << "Press Enter to Exit" << std::endl;
		std::getline(std::cin, g_filename);
	}
	else if (g_filename.ends_with(".SSQ"))
	{
		SSQ file(g_filename);
		testWrite(file);

		std::cout << "Press Enter to Exit" << std::endl;
		std::getline(std::cin, g_filename);
	}
	else if (g_filename.ends_with(".WEB"))
	{
		WEB file(g_filename);
		testWrite(file);

		std::cout << "Press Enter to Exit" << std::endl;
		std::getline(std::cin, g_filename);
	}
	else
	{
		std::cout << "Filetype not yet supported\n";
		std::cout << "Press Enter to Exit" << std::endl;
		std::getline(std::cin, g_filename);
	}
	
	return 0;
}
