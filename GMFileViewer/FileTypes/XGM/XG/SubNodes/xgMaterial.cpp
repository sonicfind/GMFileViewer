#include "xgMaterial.h"
#include "Graphics.h"

void xgMaterial::load(FilePointer& file, const XG* xg)
{
	PString::ReadNamedValue("blendType", m_blendType, file);
	PString::ReadNamedValue("shadingType", m_shadingType, file);
	PString::ReadNamedValue("diffuse", m_diffuse, file);
	PString::ReadNamedValue("specular", m_specular, file);
	PString::ReadNamedValue("flags", m_flags, file);
	PString::ReadNamedValue("textureEnv", m_textureEnv, file);
	PString::ReadNamedValue("uTile", m_uTile, file);
	PString::ReadNamedValue("vTile", m_vTile, file);

	while (XG_SubNode* node = xg->grabNode_optional("inputTexture", "outputTexture", file))
		m_inputTexture = static_cast<xgTexture*>(node);
}

void xgMaterial::bind(uint32_t slot) const
{
	if (m_inputTexture)
		m_inputTexture->bind();

	Graphics::getGraphics()->updateConstantBuffer(32 * slot, this, 32);
}

size_t xgMaterial::getNumMaterials() const
{
	return 1;
}
