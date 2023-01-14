#include "xgMultiPassMaterial.h"
#include "../PString.h"
#include "FileOperations.h"

void xgMultiPassMaterial::load(const char*& input, const XG* xg)
{
	while (XG_SubNode* node = xg->grabNode_optional("inputMaterial", "outputMaterial", input))
		m_inputMaterials.push_back(static_cast<xgMaterial*>(node));
}
