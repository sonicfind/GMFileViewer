#include "FileContainer.h"

FileContainer::FileContainer(const std::filesystem::path& path)
{
	FILE* file;
	if (_wfopen_s(&file, path.c_str(), L"rb") != 0 || !file)
		throw std::runtime_error("Error: " + path.string() + " could not be located");

	_fseek_nolock(file, 0, SEEK_END);
	m_fileSize = _ftell_nolock(file);
	_fseek_nolock(file, 0, SEEK_SET);

	m_fileData = std::make_shared<char[]>(m_fileSize + 1);
	if (_fread_nolock(m_fileData.get(), m_fileSize, 1, file) != 1)
		throw std::runtime_error("Uh, shoot");
	_fclose_nolock(file);
	m_fileData[m_fileSize] = 0;
}
