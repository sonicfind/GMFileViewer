#include "xgMultiPassMaterial.h"
#include "../PString.h"
#include "FileOperations.h"

void xgMultiPassMaterial::load(const char*& input, const std::vector<XGVectElement>& nodes)
{
	BindNodes(m_inputMaterials, "inputMaterial", "outputMaterial", input, nodes);
}
