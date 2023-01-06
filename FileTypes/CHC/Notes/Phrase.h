#pragma once
#include "Note.h"

enum class BoolEnum : uint32_t
{
	False,
	True
};

struct Phrase : public Note_Sustained
{
	BoolEnum m_start = BoolEnum::False;
	BoolEnum m_end = BoolEnum::False;
	uint32_t m_animation = 0;
	uint32_t: 32, : 32, : 32;
};
