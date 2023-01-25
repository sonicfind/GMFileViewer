#pragma once
#include "xgMaterial.h"

class xgMultiPassMaterial : public XG_MaterialNode
{
	std::vector<xgMaterial*> m_inputMaterials;

public:
	using XG_MaterialNode::XG_MaterialNode;
	void load(FilePointer& file, const XG* xg) override;

	void writeType(FileWriter& file) const override;
	void save(FileWriter& file) const override;

	void bind(size_t slot) const override;
	size_t getNumMaterials() const override;
};

