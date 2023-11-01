#include <iostream>
#include "FileTypes/XGM/XGM.h"
#include "FileTypes/CHC/CHC.h"
#include "FileTypes/SSQ/SSQ.h"
#include "FileTypes/WEB/WEB.h"
#include "OpenGL/Graphics_OGL.h"

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

void runSequence(Graphics& gfx, SSQ& sequence, XGM& pack);

int main()
{
	std::cout << "Drag and drop a file:";
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

			file.testGraphics(index - 1);
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
		SSQ sequence(g_filename);
		std::string packName;
		std::cout << "Drag and drop an XGM file:";
		std::getline(std::cin, packName);

		if (packName[0] == '\"')
			packName = packName.substr(1, packName.length() - 2);

		XGM pack(packName);

		try
		{
			Graphics_OGL gfx(g_filename, 1280, 960);
			runSequence(gfx, sequence, pack);
		}
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}

		testWrite(sequence);

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
		std::cout << "Filetype not supported\n";
		std::cout << "Press Enter to Exit" << std::endl;
		std::getline(std::cin, g_filename);
	}
	
	return 0;
}

void runSequence(Graphics& gfx, SSQ& sequence, XGM& pack)
{
	sequence.loadSequence(gfx, pack);

	constexpr float factor = 30;
	constexpr float start = 300;

	size_t count = 0;
	float prev = start;
	auto t1 = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; !gfx.shouldClose(); ++i)
	{
		auto t2 = std::chrono::high_resolution_clock::now();
		float time = factor * std::chrono::duration<float>(t2 - t1).count() + start;
		if (time >= prev + factor)
		{
			std::cout << 1000.0 / double(count) << " ms/frame\n";
			std::cout << count << " frames" << std::endl;
			count = 0;
			prev = int(time / factor) * factor;
		}
		++count;

		gfx.updateTitle(std::to_string(time));

		gfx.resetFrame();
		//sequence.mixedUpdateAndDraw(time);
		sequence.update(gfx, pack, time);
		sequence.draw(gfx, pack, false);
		sequence.draw(gfx, pack, true);
		gfx.displayFrame();
	}
}
