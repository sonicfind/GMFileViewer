#include "WEB.h"
#include <iostream>

WEB::WEB(const std::filesystem::path& filePath) : m_setup(new PlayerModel_Setup(FilePointer(filePath))) {}

void WEB::saveToFile(const std::filesystem::path& filePath) const
{
	FileWriter file(filePath);
	if (!file.isOpen())
	{
		std::cout << "File could not be opened for writing.\n";
		return;
	}

	m_setup->save(file);
}
