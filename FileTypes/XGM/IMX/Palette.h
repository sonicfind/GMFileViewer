#pragma once
#include "Pixel.h"
#include "FilePointer.h"
#include "GMArray.h"

struct HalfIndex
{
	unsigned char index1 : 4, index2 : 4;
};

template <typename T>
struct Palette
{
	GMArray_View<Pixel> m_colors;
	GMArray_View<T> m_indices;
	Palette(FilePointer& file)
	{
		m_colors.view<true>(file);
		file += 4;
		m_indices.view<true>(file);
	}

	Pixel operator[](const size_t position) const
	{
		return m_colors[m_indices[position]];
	}
};

template<>
Pixel Palette<HalfIndex>::operator[](const size_t position) const;
