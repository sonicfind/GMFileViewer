#include "FileOperations.h"

void FileOps::Read(void* loc, const char*& input, const size_t size)
{
	memcpy(loc, input, size);
	input += size;
}
