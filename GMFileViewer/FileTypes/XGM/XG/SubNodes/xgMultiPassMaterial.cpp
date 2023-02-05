#include "xgMultiPassMaterial.h"

void xgMultiPassMaterial::load(FileReader& file, const XG* xg)
{
	while (XG_SubNode* node = xg->grabNode_optional("inputMaterial", "outputMaterial", file))
		m_inputMaterials.push_back(static_cast<xgMaterial*>(node));
}

void xgMultiPassMaterial::writeType(FileWriter& file) const
{
	PString::WriteString("xgMultiPassMaterial", file);
}

void xgMultiPassMaterial::save(FileWriter& file) const
{
	for (const auto node : m_inputMaterials)
		WriteNode("inputMaterial", "outputMaterial", node, file);
}

void xgMultiPassMaterial::bind(size_t slot) const
{
	m_inputMaterials[slot]->bind(0);
}

size_t xgMultiPassMaterial::getNumMaterials() const
{
	return m_inputMaterials.size();
}

bool xgMultiPassMaterial::hasTransparency() const
{
	return m_inputMaterials[0]->hasTransparency();
}
