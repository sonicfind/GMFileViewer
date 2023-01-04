#pragma once
#include "Pixel.h"
#include "FileOperations.h"
#include "GMArray.h"

struct HalfIndex
{
	unsigned char index1 : 4, index2 : 4;
};

template <FileOps::Standard T>
struct Palette
{
	GMArray_View<Pixel> m_colors;
	GMArray_View<T> m_indices;
	Palette(const char*& input)
	{
		m_colors.read<true>(input);
		input += 4;
		m_indices.read<true>(input);
	}

	Pixel operator[](const size_t position) const
	{
		return m_colors[m_indices[position]];
	}
};

template<>
Pixel Palette<HalfIndex>::operator[](const size_t position) const;
