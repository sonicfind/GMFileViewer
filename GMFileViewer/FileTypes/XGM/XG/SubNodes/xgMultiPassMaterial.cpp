#include "xgMultiPassMaterial.h"
#include "../PString.h"
#include "FilePointer.h"

void xgMultiPassMaterial::load(FilePointer& file, const XG* xg)
{
	while (XG_SubNode* node = xg->grabNode_optional("inputMaterial", "outputMaterial", file))
		m_inputMaterials.push_back(static_cast<xgMaterial*>(node));
}
