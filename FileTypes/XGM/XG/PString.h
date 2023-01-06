#pragma once
#include <string>
#include "FileOperations.h"
#include "GMArray.h"
#include "XGM/XG/SubNodes/VertexList.h"

namespace PString
{
	void Read(std::string& str, const char*& input);
	void Read(std::string_view& str, const char*& input);
	std::string_view Read(const char*& input);

	bool CheckForString_nothrow(std::string_view strToMatch, const char*& input) noexcept;
	void CheckForString(std::string_view strToMatch, const char*& input);

	template <typename T>
	void ReadNamedValue(std::string_view name, T& value, const char*& input)
	{
		CheckForString(name, input);
		FileOps::Read(value, input);
	}

	template <typename T>
	void ReadNamedValue(std::string_view name, GMArray<T>& value, const char*& input)
	{
		CheckForString(name, input);
		value.reserve_and_fill(input);
	}

	template <>
	void ReadNamedValue(std::string_view name, std::string& value, const char*& input);

	template <>
	void ReadNamedValue(std::string_view name, VertexList& value, const char*& input);
}
