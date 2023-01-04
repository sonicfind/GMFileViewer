#pragma once
#include <stdint.h>
#include <memory>
#include <assert.h>
#include <filesystem>

namespace FileOps
{
	template<size_t SIZE>
	bool checkTag(const char (&tag)[SIZE], const char*& input)
	{
		static_assert(SIZE <= 9, "Invalid tag");
		if constexpr (SIZE <= 5)
		{
			if (strncmp(input, tag, 4) != 0)
				return false;

			input += 4;
		}
		else if (strncmp(input, tag, 8) != 0)
			return false;
		else
			input += 8;
		return true;
	}

	template<typename T>
	concept Standard = std::is_standard_layout<T>::value;

	void Read(void* loc, const char*& input, const size_t size);

	template <Standard T>
	void Read(T& value, const char*& input, const size_t size)
	{
		Read(&value, input, size);
	}

	template <Standard T>
	void Read(T& value, const char*& input)
	{
		Read(value, input, sizeof(T));
	}

	template <Standard T>
	T Read(const char*& input)
	{
		T value;
		Read(value, input);
		return value;
	}

	class FilePointers
	{
		size_t m_fileSize = 0;
		std::shared_ptr<char[]> m_fileData;
		char* m_fileEnd = nullptr;

	public:
		FilePointers(const std::filesystem::path& path)
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

			m_fileEnd = m_fileData.get() + m_fileSize;
			*m_fileEnd = 0;
		}

		const char* begin() const noexcept { return m_fileData.get(); }
		constexpr size_t length() const noexcept { return m_fileSize; }
		const char* end() const noexcept { return m_fileEnd; }
	};
}
