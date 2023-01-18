#pragma once
#include "Note.h"

enum class CurveState : uint32_t
{
	Rigid,
	Smooth
};

struct Traceline : public Note_Sustained
{
	float m_angle = 0;
	uint32_t m_smoothCurve = 0;
};
