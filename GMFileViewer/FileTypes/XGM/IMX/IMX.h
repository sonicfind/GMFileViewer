#pragma once
#include <stdint.h>
#include "Palette.h"
#include "FileWriter.h"

class IMX
{
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	std::unique_ptr<Pixel[]> m_data;

public:
	void load(FilePointer file);
	void save(FileWriter& file) const;

	void createTextureBuffer(std::string_view name) const;

private:
	template <typename T>
	void readImage_Indexed(FilePointer& file)
	{
		const Palette<T> palette(file);
		const uint32_t size = m_height * m_width;

		m_data = std::make_unique<Pixel[]>(size);
		for (uint32_t i = 0; i < size; ++i)
			m_data[i] = palette[i];
	}

	void readImage_RGB(FilePointer& file);
	void readImage_RGBA(FilePointer& file);

	struct Compression
	{
		GMArray<Pixel> palette;
	};

	void compress_palette(const Pixel* const palette, const uint32_t paletteSize, FileWriter& file) const;
	void compress_bitmap(FileWriter& file) const;
};
