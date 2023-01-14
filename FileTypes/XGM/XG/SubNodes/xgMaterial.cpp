#include "xgMaterial.h"
#include "../PString.h"
#include "FileOperations.h"

void xgMaterial::load(const char*& input, const XG* xg)
{
	PString::ReadNamedValue("blendType", m_blendType, input);
	PString::ReadNamedValue("shadingType", m_shadingType, input);
	PString::ReadNamedValue("diffuse", m_diffuse, input);
	PString::ReadNamedValue("specular", m_specular, input);
	PString::ReadNamedValue("flags", m_flags, input);
	PString::ReadNamedValue("textureEnv", m_textureEnv, input);
	PString::ReadNamedValue("uTile", m_uTile, input);
	PString::ReadNamedValue("vTile", m_vTile, input);

	while (XG_SubNode* node = xg->grabNode_optional("inputTexture", "outputTexture", input))
		m_inputTexture = static_cast<xgTexture*>(node);
}
