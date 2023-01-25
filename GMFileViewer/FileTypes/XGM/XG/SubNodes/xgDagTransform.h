#pragma once
#include "xgBgMatrix.h"
class xgDagTransform : public XG_SubNode
{
	xgBgMatrix* m_inputMatrix = nullptr;

public:
	using XG_SubNode::XG_SubNode;
	void load(FileReader& file, const XG* xg) override;

	void writeType(FileWriter& file) const override;
	void save(FileWriter& file) const override;

	DirectX::XMMATRIX calcTransformMatrix() const;
};

