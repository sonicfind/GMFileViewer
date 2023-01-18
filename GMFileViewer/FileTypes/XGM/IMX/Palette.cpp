#include "Palette.h"

template<>
Pixel Palette<HalfIndex>::operator[](const size_t position) const
{
	const HalfIndex half = m_indices[position / 2];
	if (position & 1)
		return m_colors[half.index2];
	else
		return m_colors[half.index1];
}