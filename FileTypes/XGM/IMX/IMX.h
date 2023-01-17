#pragma once
#include <stdint.h>
#include "Palette.h"

class IMX
{
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	std::unique_ptr<Pixel[]> m_data;

public:
	void load(FilePointer file);

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
};
