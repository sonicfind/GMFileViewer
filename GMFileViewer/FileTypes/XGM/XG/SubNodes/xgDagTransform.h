#pragma once
#include "xgBgMatrix.h"
class xgDagTransform : public XG_SubNode
{
	xgBgMatrix* m_inputMatrix = nullptr;

public:
	void load(FilePointer& file, const XG* xg) override;
	DirectX::XMMATRIX calcTransformMatrix() const;
};

