#include "xgMaterial.h"
#include "Graphics.h"

void xgMaterial::load(FileReader& file, const XG* xg)
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

void xgMaterial::writeType(FileWriter& file) const
{
	PString::WriteString("xgMaterial", file);
}

void xgMaterial::save(FileWriter& file) const
{
	PString::WriteNamedValue("blendType", m_blendType, file);
	PString::WriteNamedValue("shadingType", m_shadingType, file);
	PString::WriteNamedValue("diffuse", m_diffuse, file);
	PString::WriteNamedValue("specular", m_specular, file);
	PString::WriteNamedValue("flags", m_flags, file);
	PString::WriteNamedValue("textureEnv", m_textureEnv, file);
	PString::WriteNamedValue("uTile", m_uTile, file);
	PString::WriteNamedValue("vTile", m_vTile, file);

	if (m_inputTexture)
		WriteNode("inputTexture", "outputTexture", m_inputTexture, file);
}

void xgMaterial::bind(size_t slot) const
{
	const GraphicsInstance gfx = Graphics::getGraphics();
	if (m_inputTexture)
	{
		m_inputTexture->bind();
		gfx->setShaderInt("useTexture", 1);
	}
	else
		gfx->setShaderInt("useTexture", 0);

	
	gfx->bindConstantBuffer(Graphics::Material);
	gfx->updateConstantBuffer(0, &m_blendType, 64);

	if (!hasTransparency())
	{
		gfx->enable(Graphics::Depth_Mask);
		gfx->disable(Graphics::AlphaBlending);
	}
	else
	{
		gfx->disable(Graphics::Depth_Mask);
		gfx->enable(Graphics::AlphaBlending);
		gfx->setBlendFunc(m_blendType);
	}
}

size_t xgMaterial::getNumMaterials() const
{
	return 1;
}

bool xgMaterial::hasTransparency() const
{
	return m_blendType != Blending::Opaque || (m_flags & 1);
}
