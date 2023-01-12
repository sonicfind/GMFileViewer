#pragma once
#include "SSQModelType.h"

class XGEntry
{
	alignas(16) char m_name[16];
	uint32_t m_isClone;
	uint32_t m_cloneID;
	uint32_t m_unknown;

	SSQModelType m_type;
	float m_length;
	float m_speed;
	float m_framerate;

public:
	SSQModelType getModelType() const { return m_type; }
};
