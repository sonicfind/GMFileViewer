#pragma once
#include "xgMaterial.h"

class xgMultiPassMaterial : public XG_MaterialNode
{
	std::vector<xgMaterial*> m_inputMaterials;

public:
	void load(const char*& input, const std::vector<XGVectElement>& nodes) override;
};

