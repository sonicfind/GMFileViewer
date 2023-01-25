#pragma once
#include <string>
#include "FileReader.h"
#include "FileWriter.h"
#include "GMArray.h"

namespace PString
{
	void GetString(std::string_view& str, FileReader& file);
	std::string_view GetString(FileReader& file);

	void WriteString(std::string_view str, FileWriter& file);

	bool CheckForString(std::string_view strToMatch, FileReader& file) noexcept;
	void ThrowOnStringMismatch(std::string_view strToMatch, FileReader& file);

	template <typename T>
	void ReadNamedValue(std::string_view name, T& value, FileReader& file)
	{
		ThrowOnStringMismatch(name, file);
		file.read(value);
	}

	template <typename T>
	void ReadNamedValue(std::string_view name, GMArray<T>& value, FileReader& file)
	{
		ThrowOnStringMismatch(name, file);
		value.reserve_and_fill(file);
	}

	template<typename T>
	void WriteNamedValue(std::string_view name, const T& value, FileWriter& file)
	{
		WriteString(name, file);
		file << value;
	}

	template <typename T>
	void WriteNamedValue(std::string_view name, const GMArray<T>& value, FileWriter& file)
	{
		WriteString(name, file);
		value.write_full(file);
	}
}
