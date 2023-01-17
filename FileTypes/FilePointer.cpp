#include "FilePointer.h"

FilePointer::FilePointer(const std::filesystem::path& path)
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

	m_currentPosition = m_fileData.get();
	m_fileEnd = m_currentPosition + m_fileSize;
}

FilePointer::FilePointer(const FilePointer& file) : m_fileSize(file.m_fileSize), m_currentPosition(file.m_currentPosition), m_fileEnd(file.m_fileEnd) {}

FilePointer& FilePointer::operator+=(size_t amount)
{
	m_currentPosition += amount;
	if (m_currentPosition > m_fileEnd)
		throw std::runtime_error("FilePointer out of bounds");
	return *this;
}

FilePointer& FilePointer::operator++()
{
	operator+=(1);
	return *this;
}

FilePointer FilePointer::operator++(int)
{
	FilePointer copy(*this);
	operator+=(1);
	return copy;
}



const char* FilePointer::get() const { return m_currentPosition; }
char FilePointer::operator*() const { return *m_currentPosition; }
