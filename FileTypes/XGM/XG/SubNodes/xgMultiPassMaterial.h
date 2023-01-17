#pragma once
#include "xgMaterial.h"
#include <vector>

class xgMultiPassMaterial : public XG_MaterialNode
{
	std::vector<xgMaterial*> m_inputMaterials;

public:
	void load(FilePointer& file, const XG* xg) override;
};

