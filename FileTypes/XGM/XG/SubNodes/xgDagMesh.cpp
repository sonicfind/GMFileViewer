#include "xgDagMesh.h"

void xgDagMesh::load(const char*& input, const std::vector<XGVectElement>& nodes)
{
	PString::ReadNamedValue("primType", m_primType, input);

	m_prim.load(input);
	m_triFan.load(input);
	m_triStrip.load(input);
	m_triList.load(input);

	PString::ReadNamedValue("cullFunc", m_cullFunc, input);
	while (BindNode_optional(m_inputGeometry, "inputGeometry", "outputGeometry", input, nodes) ||
           BindNode_optional(m_inputMaterial, "inputMaterial", "outputMaterial", input, nodes));
}
