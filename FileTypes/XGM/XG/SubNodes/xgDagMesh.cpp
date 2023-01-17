#include "xgDagMesh.h"

void xgDagMesh::load(FilePointer& file, const XG* xg)
{
	PString::ReadNamedValue("primType", m_primType, file);

	m_prim.load(file);
	m_triFan.load(file);
	m_triStrip.load(file);
	m_triList.load(file);

	PString::ReadNamedValue("cullFunc", m_cullFunc, file);
	while (xg->grabNode_nondestructive(m_inputGeometry, "inputGeometry", "outputGeometry", file) ||
		   xg->grabNode_nondestructive(m_inputMaterial, "inputMaterial", "outputMaterial", file));
}

void xgDagMesh::update() const
{
	m_inputGeometry->update();
}
