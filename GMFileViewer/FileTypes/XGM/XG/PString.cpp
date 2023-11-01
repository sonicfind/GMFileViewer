#include "PString.h"

std::string_view PString::GetString(FileReader& file)
{
	unsigned char length = *file++;
	const char* start = file.get();
	file += length;
	return std::string_view(start, length);
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

