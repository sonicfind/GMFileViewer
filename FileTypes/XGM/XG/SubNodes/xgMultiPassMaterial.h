#pragma once
#include "xgMaterial.h"
#include <vector>

class xgMultiPassMaterial : public XG_MaterialNode
{
	std::vector<xgMaterial*> m_inputMaterials;

public:
	void load(const char*& input, const XG* xg) override;
};

