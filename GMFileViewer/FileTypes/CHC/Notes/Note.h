#pragma once
#include <stdint.h>

struct Note
{
	int32_t m_pivotAlpha = 0;
};

struct Note_Sustained : public Note
{
	uint32_t m_duration = 1;
};
