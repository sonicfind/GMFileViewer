#pragma once
#include "xgBgMatrix.h"

class xgBone : public XG_SubNode
{
	glm::mat4 m_restMatrix;
	xgBgMatrix* m_inputMatrix = nullptr;

public:
	using XG_SubNode::XG_SubNode;
	void load(FileReader& file, const XG* xg) override;

	void writeType(FileWriter& file) const override;
	void save(FileWriter& file) const override;

	glm::mat4 calcTransformMatrix() const;
};

