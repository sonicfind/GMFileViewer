#include "xgMaterial.h"
#include "../PString.h"
#include "FileOperations.h"

void xgMaterial::load(const char*& input, const std::vector<XGVectElement>& nodes)
{
	PString::ReadNamedValue("blendType", m_blendType, input);
	PString::ReadNamedValue("shadingType", m_shadingType, input);
	PString::ReadNamedValue("diffuse", m_diffuse, input);
	PString::ReadNamedValue("specular", m_specular, input);
	PString::ReadNamedValue("flags", m_flags, input);
	PString::ReadNamedValue("textureEnv", m_textureEnv, input);
	PString::ReadNamedValue("uTile", m_uTile, input);
	PString::ReadNamedValue("vTile", m_vTile, input);

	while (BindNode_optional(m_inputTexture, "inputTexture", "outputTexture", input, nodes));
}
