#pragma once
#include "SSQModelType.h"

class XGEntry
{
	alignas(16) char m_name[16];
	uint32_t m_isClone;
	uint32_t m_cloneID;
	ModelDrawStatus m_drawStatus;

	SSQModelType m_type;
	float m_length;
	float m_speed;
	float m_framerate;

public:
	std::string_view getName() const { return m_name; }
	bool isClone() const { return m_isClone; }
	uint32_t getCloneID() const { return m_cloneID; }
	SSQModelType getModelType() const { return m_type; }
	void setStatus(ModelDrawStatus status) { m_drawStatus = status; }
	ModelDrawStatus getStatus() const { return m_drawStatus; }
};
