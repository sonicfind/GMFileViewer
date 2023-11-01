#pragma once
#include <memory>
#include <filesystem>

class FileContainer
{
	size_t m_fileSize = 0;
	std::shared_ptr<char[]> m_fileData;

public:
	FileContainer(const std::filesystem::path& path);
	char* Get() const { return m_fileData.get(); }
	size_t Size() const { return m_fileSize; }
};
