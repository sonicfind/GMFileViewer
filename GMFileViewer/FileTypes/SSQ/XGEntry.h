#pragma once
#include "SSQModelType.h"

class XGEntry
{
	alignas(16) char m_name[16];
	uint32_t m_isClone;
	uint32_t m_cloneID;
	uint32_t m_modelIndex;

	SSQModelType m_type;
	float m_length;
	float m_speed;
	float m_framerate;

public:
	std::string_view getName() const { return m_name; }
	void setModelIndex(uint32_t index) { m_modelIndex = index; }
	uint32_t getModelIndex() const { return m_modelIndex; }
	bool isClone() const { return m_isClone; }
	uint32_t getCloneID() const { return m_cloneID; }
	SSQModelType getModelType() const { return m_type; }
};
