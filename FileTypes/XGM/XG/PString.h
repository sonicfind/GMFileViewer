#pragma once
#include <string>
#include "FileOperations.h"

namespace PString
{
	void Read(std::string& str, const char*& input);
	void Read(std::string_view& str, const char*& input);
	std::string_view Read(const char*& input);

	bool CheckForString_nothrow(std::string_view strToMatch, const char*& input) noexcept;
	void CheckForString(std::string_view strToMatch, const char*& input);

	template <FileOps::Standard T>
	void ReadNamedValue(std::string_view name, T& value, const char*& input)
	{
		CheckForString(name, input);
		FileOps::Read(value, input);
	} 
}
