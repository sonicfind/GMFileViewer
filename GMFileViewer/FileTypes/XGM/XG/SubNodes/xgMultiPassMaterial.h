#pragma once
#include "xgMaterial.h"

class xgMultiPassMaterial : public XG_SubNode, public XG_MaterialNode
{
	std::vector<xgMaterial*> m_inputMaterials;

public:
	void load(FilePointer& file, const XG* xg) override;
	void bind(uint32_t slot) const override;
};

