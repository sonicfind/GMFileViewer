#pragma once
#include "Note.h"

enum class Button_Direction : uint32_t
{
	Left,
	Down,
	Right,
	Up
};

struct Guard : public Note
{
	Button_Direction m_button;
	uint32_t: 32, : 32;
};
