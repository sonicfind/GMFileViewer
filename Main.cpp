#include "FileTypes/XGM/XGM.h"
#include <iostream>

int main()
{
	std::string filename;
	std::getline(std::cin, filename);

	if (filename[0] == '\"')
		filename = filename.substr(1, filename.length() - 2);

	std::filesystem::path path(filename);

	if (path.extension() == U".XGM")
	{
		auto t1 = std::chrono::high_resolution_clock::now();
			XGM file(path);
		auto t2 = std::chrono::high_resolution_clock::now();
		std::cout << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000 << " milliseconds\n";
		std::getline(std::cin, filename);
	}
	return 0;
}