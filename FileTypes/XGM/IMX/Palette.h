#pragma once
#include "Pixel.h"
#include "FileOperations.h"

struct HalfIndex
{
	unsigned char index1 : 4, index2 : 4;
};

template <FileOps::Standard T>
struct Palette
{
	std::unique_ptr<Pixel[]> m_colors;
	std::unique_ptr<T[]> m_indices;
	Palette(const char*& input) : m_colors(FileOps::Read_Array<Pixel>(input))
	{
		input += 4;
		m_indices = FileOps::Read_Array<T>(input);
	}

	Pixel operator[](const size_t position) const
	{
		return m_colors[m_indices[position]];
	}
};

template<>
Pixel Palette<HalfIndex>::operator[](const size_t position) const;
