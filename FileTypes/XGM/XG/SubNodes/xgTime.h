#pragma once
#include "XG_SubNode.h"
class xgTime : public XG_SubNode
{
	float m_numFrames = 0;
	float m_time = 0;

public:
	void load(const char*& input, const std::vector<XGVectElement>& nodes) override;
};

