#include "PString.h"

void PString::Read(std::string& str, const char*& input)
{
	unsigned char length = *input++;
	str.assign(input, length);
	input += length;
}

void PString::Read(std::string_view& str, const char*& input)
{
	unsigned char length = *input++;
	str = std::string_view(input, length);
	input += length;
}

std::string_view PString::Read(const char*& input)
{
	std::string_view str;
	Read(str, input);
	return str;
}

bool PString::CheckForString_nothrow(std::string_view strToMatch, const char*& input) noexcept
{
	unsigned char length = *input;
	if (strToMatch != std::string_view(input + 1, length))
		return false;

	input += length + 1ULL;
	return true;
}

void PString::CheckForString(std::string_view strToMatch, const char*& input)
{
	if (!CheckForString_nothrow(strToMatch, input))
		throw "PString mismatch";
}
