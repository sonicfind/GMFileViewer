#include "IMX.h"
#include "Graphics.h"

void IMX::load(FilePointer file)
{
	if (!file.checkTag("IMX"))
		throw "IMX file read error";

	file += 16;
	file.read(m_width);
	file.read(m_height);

	const uint32_t pixelVal1 = file.read<uint32_t>();
	const uint32_t pixelVal2 = file.read<uint32_t>();

	if (pixelVal1 == 0 && pixelVal2 == 0)
		readImage_Indexed<HalfIndex>(file);
	else if (pixelVal1 == 1 && pixelVal2 == 1)
		readImage_Indexed<unsigned char>(file);
	else if (pixelVal2 == 2)
	{
		if (pixelVal1 == 3)
			readImage_RGB(file);
		else if (pixelVal1 == 4)
			readImage_RGBA(file);
		else
			throw "Unknown Pixel Storage value combination";
	}
	else
		throw "Unknown Pixel Storage value combination";
}

void IMX::readImage_RGB(FilePointer& file)
{
	const auto size = file.read<uint32_t>() / 3;
	m_data = std::make_unique<Pixel[]>(size);
	for (uint32_t i = 0; i < size; ++i)
		file.read(m_data[i], 3);		
}

void IMX::readImage_RGBA(FilePointer& file)
{
	const auto size = file.read<uint32_t>();
	m_data = std::make_unique<Pixel[]>(size / 4);
	file.read(m_data.get(), size);
}

void IMX::createTextureBuffer(std::string_view name) const
{
	Graphics::getGraphics()->createTexture(name, m_data.get(), m_width, m_height);
}
