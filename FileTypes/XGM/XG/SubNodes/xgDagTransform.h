#pragma once
#include "xgBgMatrix.h"
class xgDagTransform : public XG_SubNode
{
	xgBgMatrix* m_inputMatrix = nullptr;

public:
	void load(const char*& input, const std::vector<XGVectElement>& nodes) override;
};

