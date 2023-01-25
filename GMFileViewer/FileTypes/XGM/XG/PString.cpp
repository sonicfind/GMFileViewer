#include "PString.h"

void PString::GetString(std::string_view& str, FileReader& file)
{
	unsigned char length = *file++;
	str = std::string_view(file.get(), length);
	file += length;
}

std::string_view PString::GetString(FileReader& file)
{
	std::string_view str;
	GetString(str, file);
	return str;
}

void PString::WriteString(std::string_view str, FileWriter& file)
{
	file << unsigned char(str.length());
	file.write(str.data(), str.length());
}

bool PString::CheckForString(std::string_view strToMatch, FileReader& file) noexcept
{
	unsigned char length = *file;
	if (strToMatch != std::string_view(file.get() + 1, length))
		return false;

	file += length + 1ULL;
	return true;
}

void PString::ThrowOnStringMismatch(std::string_view strToMatch, FileReader& file)
{
	if (!CheckForString(strToMatch, file))
		throw "PString mismatch";
}

