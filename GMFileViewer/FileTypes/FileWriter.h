#pragma once
#include <memory>
#include <filesystem>
#include <fstream>

class FileWriter
{
	std::ofstream m_file;

public:
	FileWriter(const std::filesystem::path& path);
	bool isOpen() const;

	template<size_t SIZE>
	void writeTag(const char(&tag)[SIZE])
	{
		static_assert(SIZE <= 9, "Invalid tag");

		static char tagBuffer[9];
		memcpy(tagBuffer, tag, SIZE);

		if constexpr (SIZE <= 5)
			m_file.write(tagBuffer, 4);
		else 
			m_file.write(tagBuffer, 8);
	}

	template <typename T>
	void write(T* src, const size_t size)
	{
		m_file.write((char*)src, size);
	}

	template <typename T>
	void write(T& value, const size_t size)
	{
		write(&value, size);
	}

	template <typename T>
	void write(T& value)
	{
		write(value, sizeof(T));
	}

	template <typename T>
	friend FileWriter& operator<<(FileWriter& file, const T& value)
	{
		file.write(value);
		return file;
	}

	std::streampos tell();
	void seek(std::streampos pos);

};
