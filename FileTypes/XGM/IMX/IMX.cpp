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

	FileOps::Read(m_pixelVal1, input);
	FileOps::Read(m_pixelVal2, input);

	if (m_pixelVal1 == 0 && m_pixelVal2 == 0)
		readImage_Indexed<HalfIndex>(input);
	else if (m_pixelVal1 == 1 && m_pixelVal2 == 1)
		readImage_Indexed<unsigned char>(input);
	else if (m_pixelVal2 == 2)
	{
		if (m_pixelVal1 == 3)
			readImage_RGB(input);
		else if (m_pixelVal1 == 4)
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
	if (m_data.reserve(size))
		for (auto& pixel : m_data)
			FileOps::Read(pixel, input, 3);
}

void IMX::readImage_RGBA(const char* input)
{
	m_data.reserve_and_fill<true>(input);
}
