#include "xgMultiPassMaterial.h"

void xgMultiPassMaterial::load(FilePointer& file, const XG* xg)
{
	while (XG_SubNode* node = xg->grabNode_optional("inputMaterial", "outputMaterial", file))
		m_inputMaterials.push_back(static_cast<xgMaterial*>(node));
}

void xgMultiPassMaterial::bind(uint32_t slot) const
{
	for (const auto* material : m_inputMaterials)
	{
		material->bind(slot);
		++slot;
		/* Change Material & Texture slot */
	}
}

size_t xgMultiPassMaterial::getNumMaterials() const
{
	return m_inputMaterials.size();
}
