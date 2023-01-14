#include "xgDagMesh.h"

void xgDagMesh::load(const char*& input, const XG* xg)
{
	PString::ReadNamedValue("primType", m_primType, input);

	m_prim.load(input);
	m_triFan.load(input);
	m_triStrip.load(input);
	m_triList.load(input);

	PString::ReadNamedValue("cullFunc", m_cullFunc, input);
	while (xg->grabNode_nondestructive(m_inputGeometry, "inputGeometry", "outputGeometry", input) ||
		   xg->grabNode_nondestructive(m_inputMaterial, "inputMaterial", "outputMaterial", input));
}
