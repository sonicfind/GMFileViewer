#pragma once
#include <string>
#include "FilePointer.h"
#include "FileWriter.h"
#include "GMArray.h"

namespace PString
{
	void GetString(std::string_view& str, FilePointer& file);
	std::string_view GetString(FilePointer& file);

	void WriteString(std::string_view str, FileWriter& file);

	bool CheckForString(std::string_view strToMatch, FilePointer& file) noexcept;
	void ThrowOnStringMismatch(std::string_view strToMatch, FilePointer& file);

	template <typename T>
	void ReadNamedValue(std::string_view name, T& value, FilePointer& file)
	{
		ThrowOnStringMismatch(name, file);
		file.read(value);
	}

	template <typename T>
	void ReadNamedValue(std::string_view name, GMArray<T>& value, FilePointer& file)
	{
		ThrowOnStringMismatch(name, file);
		value.reserve_and_fill(file);
	}

	template <>
	void ReadNamedValue(std::string_view name, char (&str)[16], FilePointer& file);

	template<typename T>
	void WriteNamedValue(std::string_view name, const T& value, FileWriter& file)
	{
		WriteString(name, file);
		file << value;
	}
}
