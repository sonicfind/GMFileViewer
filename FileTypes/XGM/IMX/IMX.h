#pragma once
#include <stdint.h>
#include "Palette.h"

class IMX
{
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	uint32_t m_pixelVal1 = 0;
	uint32_t m_pixelVal2 = 0;
	GMArray<Pixel> m_data;

public:
	void load(const char* input);

private:
	template <FileOps::Standard T>
	void readImage_Indexed(const char* input)
	{
		const Palette<T> palette(input);
		m_data.init(m_height * m_width);
		
		for (uint32_t i = 0; i < m_data.getSize(); ++i)
			m_data[i] = palette[i];
	}

	void readImage_RGB(const char* input);
	void readImage_RGBA(const char* input);
};
