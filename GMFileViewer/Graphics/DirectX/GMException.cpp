#include "GMException.h"
#include <sstream>

GMException::GMException(int line, const char* file) noexcept
	: line(line), file(file)
{
}

const char* GMException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* GMException::GetType() const noexcept
{
	return "GM Exception";
}

int GMException::GetLine() const noexcept
{
	return line;
}

const std::string& GMException::GetFile() const noexcept
{
	return file;
}

std::string GMException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}
