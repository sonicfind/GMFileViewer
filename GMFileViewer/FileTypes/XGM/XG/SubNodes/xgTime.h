#pragma once
#include "../XG.h"
class xgTime : public XG_SubNode
{
	float m_numFrames = 0;
	float m_time = 0;

public:
	using XG_SubNode::XG_SubNode;
	void load(FilePointer& file, const XG* xg) override;

	void writeType(FileWriter& file) const override;
	void save(FileWriter& file) const override;

	void setTime(float time);
	float getTime() const noexcept;
};

