#pragma once
#include <stdint.h>
#include "Palette.h"

class IMX
{
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	std::unique_ptr<Pixel[]> m_data;

public:
	void load(const char* input);

private:
	template <typename T>
	void readImage_Indexed(const char* input)
	{
		const Palette<T> palette(input);
		const uint32_t size = m_height * m_width;

		m_data = std::make_unique<Pixel[]>(size);
		for (uint32_t i = 0; i < size; ++i)
			m_data[i] = palette[i];
	}

	void readImage_RGB(const char* input);
	void readImage_RGBA(const char* input);
};
