#pragma once
#include "xgBgMatrix.h"

class xgBone : public XG_SubNode
{
	DirectX::XMMATRIX m_restMatrix;
	xgBgMatrix* m_inputMatrix = nullptr;

public:
	void load(const char*& input, const std::vector<XGVectElement>& nodes) override;
};

