#pragma once
#include <string>
#include "FilePointer.h"
#include "GMArray.h"
#include "XGM/XG/SubNodes/VertexList.h"

namespace PString
{
	void Read(std::string_view& str, FilePointer& file);
	std::string_view Read(FilePointer& file);

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

	template <>
	void ReadNamedValue(std::string_view name, VertexList& value, FilePointer& file);
}
