#pragma once
#include "xgBgMatrix.h"

class xgBone : public XG_SubNode
{
	DirectX::XMMATRIX m_restMatrix;
	xgBgMatrix* m_inputMatrix = nullptr;

public:
	void load(FilePointer& file, const XG* xg) override;
	DirectX::XMMATRIX calcTransformMatrix() const;
};

