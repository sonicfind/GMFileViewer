#pragma once
#include <compare>

struct Pixel
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha = 1;

	friend auto operator<=>(const Pixel& lhs, const Pixel& rhs)
	{
		if (lhs.red != rhs.red)
			return lhs.red <=> rhs.red;

		if (lhs.green != rhs.green)
			return lhs.green <=> rhs.green;

		if (lhs.blue != rhs.blue)
			return lhs.blue <=> rhs.blue;

		return lhs.alpha <=> rhs.alpha;
	}

	friend bool operator==(const Pixel& lhs, const Pixel& rhs)
	{
		return lhs.red == rhs.red &&
			lhs.green == rhs.green &&
			lhs.blue == rhs.blue &&
			lhs.alpha == rhs.alpha;
	}

	friend bool operator!=(const Pixel& lhs, const Pixel& rhs)
	{
		return lhs.red != rhs.red ||
			lhs.green != rhs.green ||
			lhs.blue != rhs.blue ||
			lhs.alpha != rhs.alpha;
	}
};
