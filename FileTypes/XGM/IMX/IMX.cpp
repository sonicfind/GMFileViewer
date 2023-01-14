#include "IMX.h"
#include "FileOperations.h"
#include <memory>

void IMX::load(const char* input)
{
	if (!FileOps::checkTag("IMX", input))
		throw "IMX file read error";

	input += 16;
	FileOps::Read(m_width, input);
	FileOps::Read(m_height, input);

	const uint32_t pixelVal1 = FileOps::Read<uint32_t>(input);
	const uint32_t pixelVal2 = FileOps::Read<uint32_t>(input);

	if (pixelVal1 == 0 && pixelVal2 == 0)
		readImage_Indexed<HalfIndex>(input);
	else if (pixelVal1 == 1 && pixelVal2 == 1)
		readImage_Indexed<unsigned char>(input);
	else if (pixelVal2 == 2)
	{
		if (pixelVal1 == 3)
			readImage_RGB(input);
		else if (pixelVal1 == 4)
			readImage_RGBA(input);
		else
			throw "Unknown Pixel Storage value combination";
	}
	else
		throw "Unknown Pixel Storage value combination";
}

void IMX::readImage_RGB(const char* input)
{
	const auto size = FileOps::Read<uint32_t>(input) / 3;
	m_data = std::make_unique<Pixel[]>(size);
	for (uint32_t i = 0; i < size; ++i)
		FileOps::Read(m_data[i], input, 3);		
}

void IMX::readImage_RGBA(const char* input)
{
	const auto size = FileOps::Read<uint32_t>(input);
	m_data = std::make_unique<Pixel[]>(size / 4);
	memcpy(m_data.get(), input, size);
}
