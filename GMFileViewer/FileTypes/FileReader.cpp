#include "FileReader.h"

FileReader::FileReader(const std::filesystem::path& path)
	: file(path)
{
	m_currentPosition = file.Get();
	m_fileEnd = m_currentPosition + file.Size();
}

void FileReader::move(size_t amount)
{
	m_currentPosition += amount;
	if (m_currentPosition > m_fileEnd)
		throw std::runtime_error("FileReader out of bounds");
}

FileReader& FileReader::operator+=(size_t amount)
{
	move(amount);
	return *this;
}

FileReader& FileReader::operator++()
{
	return operator+=(1);
}

FileReader FileReader::operator++(int)
{
	FileReader copy(*this);
	move(1);
	return copy;
}

const char* FileReader::get() const { return m_currentPosition; }
char FileReader::operator*() const { return *m_currentPosition; }
