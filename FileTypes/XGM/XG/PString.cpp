#include "PString.h"

void PString::Read(std::string& str, FilePointer& file)
{
	unsigned char length = *file++;
	str.assign(file.get(), length);
	file += length;
}

void PString::Read(std::string_view& str, FilePointer& file)
{
	unsigned char length = *file++;
	str = std::string_view(file.get(), length);
	file += length;
}

std::string_view PString::Read(FilePointer& file)
{
	std::string_view str;
	Read(str, file);
	return str;
}

bool PString::CheckForString_nothrow(std::string_view strToMatch, FilePointer& file) noexcept
{
	unsigned char length = *file;
	if (strToMatch != std::string_view(file.get() + 1, length))
		return false;

	file += length + 1ULL;
	return true;
}

void PString::CheckForString(std::string_view strToMatch, FilePointer& file)
{
	if (!CheckForString_nothrow(strToMatch, file))
		throw "PString mismatch";
}

template <>
void PString::ReadNamedValue(std::string_view name, std::string& value, FilePointer& file)
{
	CheckForString(name, file);
	PString::Read(value, file);
}

template <>
void PString::ReadNamedValue(std::string_view name, VertexList& value, FilePointer& file)
{
	CheckForString(name, file);
	value.load(file);
}
