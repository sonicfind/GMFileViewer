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

void xgMaterial::bind(size_t slot) const
{
	const Graphics* gfx = Graphics::getGraphics();
	if (m_inputTexture)
	{
		m_inputTexture->bind();
		gfx->setShaderInt("useTexture", 1);
	}
	else
		gfx->setShaderInt("useTexture", 0);

	
	gfx->bindConstantBuffer(Graphics::Material);
	gfx->updateConstantBuffer(0, &m_blendType, 64);

	gfx->setBlendFunc(m_blendType);
}

size_t xgMaterial::getNumMaterials() const
{
	return 1;
}
