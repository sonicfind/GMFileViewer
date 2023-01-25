#include "IMX.h"
#include "Graphics.h"

void IMX::load(FileReader file)
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

void IMX::save(FileWriter& file) const
{
	file.writeTag("IMX");

	const char md5[16]{};
	file.write(md5);
	file << m_width << m_height;
	
	uint32_t size = m_width * m_height;

	static Pixel palette[256];
	uint32_t paletteSize = 0;
	for (uint32_t index = 0; index < size; ++index)
	{
		const Pixel& pixel = m_data[index];
		auto iter = std::lower_bound(palette, palette + paletteSize, pixel);

		if (iter == palette + paletteSize || *iter != pixel)
		{
			memmove(iter + 1, iter, sizeof(Pixel) * (palette + paletteSize - iter));
			*iter = pixel;
			++paletteSize;
			if (paletteSize > 256)
			{
				compress_bitmap(file);
				goto FileEnd;
			}
		}
	}

	compress_palette(palette, paletteSize, file);
FileEnd:
	file << uint32_t(3) << uint32_t(0);
}

void IMX::readImage_RGB(FileReader& file)
{
	const auto size = file.read<uint32_t>() / 3;
	m_data = std::make_unique<Pixel[]>(size);
	for (uint32_t i = 0; i < size; ++i)
		file.read(m_data[i], 3);		
}

void IMX::readImage_RGBA(FileReader& file)
{
	const auto size = file.read<uint32_t>();
	m_data = std::make_unique<Pixel[]>(size / 4);
	file.read(m_data.get(), size);
}

void IMX::compress_palette(const Pixel(&palette)[256], const uint32_t paletteSize, FileWriter& file) const
{
	auto getIndex = [begin = palette, end = palette + paletteSize, data = m_data.get()](uint32_t index) -> unsigned char
	{
		return (unsigned char)(std::lower_bound(begin, end, data[index]) - begin);
	};

	uint32_t size = m_width * m_height;
	if (paletteSize <= 16)
	{
		file << uint32_t(0) << uint32_t(0) << uint32_t(sizeof(Pixel) * 16);
		file.write(palette, sizeof(Pixel) * 16);
		file << uint32_t(2) << (size + 1) / 2; // Accounts for odd number size

		for (uint32_t i = 0; i < size;)
		{
			HalfIndex indices;
			indices.index1 = getIndex(i++);

			if (i < size)
				indices.index2 = getIndex(i++);

			file << indices;
		}
	}
	else
	{
		file << uint32_t(1) << uint32_t(1) << uint32_t(sizeof(Pixel) * 256);
		file.write(palette);
		file << uint32_t(2) << size;

		for (uint32_t i = 0; i < size; ++i)
			file << getIndex(i);
	}
}

void IMX::compress_bitmap(FileWriter& file) const
{
	uint32_t size = m_width * m_height;
	for (uint32_t index = 0; index < size; ++index)
	{
		if (m_data[index].alpha < 1)
		{
			file << uint32_t(4) << uint32_t(2) << 4 * size;
			file.write(m_data.get(), 4ULL * size);
			return;
		} 
	}

	file << uint32_t(3) << uint32_t(2) << 3 * size;
	for (uint32_t index = 0; index < size; ++index)
		file.write(&m_data[index], 3ULL);
}

void IMX::createTextureBuffer(std::string_view name) const
{
	Graphics::getGraphics()->createTexture(name, m_data.get(), m_width, m_height);
}
