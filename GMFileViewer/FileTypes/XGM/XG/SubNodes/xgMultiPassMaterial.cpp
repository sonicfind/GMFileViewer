#include "xgMultiPassMaterial.h"

void xgMultiPassMaterial::load(FilePointer& file, const XG* xg)
{
	while (XG_SubNode* node = xg->grabNode_optional("inputMaterial", "outputMaterial", file))
		m_inputMaterials.push_back(static_cast<xgMaterial*>(node));
}

void xgMultiPassMaterial::bind(size_t slot) const
{
	m_inputMaterials[slot]->bind(0);
}

size_t xgMultiPassMaterial::getNumMaterials() const
{
	return m_inputMaterials.size();
}
