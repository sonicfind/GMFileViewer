#pragma once
#include <memory>
#include <filesystem>

template <typename T>
[[nodiscard]] T Parse(const char*& input)
{
	T value;
	memcpy(&value, input, sizeof(T));
	input += sizeof(T);
	return value;
}

class FilePointer
{
	size_t m_fileSize = 0;
	std::shared_ptr<char[]> m_fileData;

	const char* m_currentPosition = nullptr;
	const char* m_fileEnd = nullptr;

public:
	FilePointer(const std::filesystem::path& path);
	FilePointer(const FilePointer& file);

	template<size_t SIZE>
	[[nodiscard]] bool checkTag(const char(&tag)[SIZE])
	{
		static_assert(SIZE <= 9, "Invalid tag");
		if constexpr (SIZE <= 5)
		{
			if (strncmp(m_currentPosition, tag, 4) != 0)
				return false;
			move(4);
		}
		else if (strncmp(m_currentPosition, tag, 8) != 0)
			return false;
		else
			move(8);
		return true;
	}

	template <typename T>
	void read(T* dst, const size_t size)
	{
		memcpy(dst, m_currentPosition, size);
		move(size);
	}

	template <typename T>
	void read(T& value, const size_t size)
	{
		read(&value, size);
	}

	template <typename T>
	void read(T& value)
	{
		read(value, sizeof(T));
	}

	template <typename T>
	[[nodiscard]] T read()
	{
		T value;
		read(value);
		return value;
	}

	void move(size_t amount);
	FilePointer& operator+=(size_t amount);
	FilePointer& operator++();
	FilePointer operator++(int);
	[[nodiscard]] const char* get() const;
	[[nodiscard]] char operator*() const;

};
