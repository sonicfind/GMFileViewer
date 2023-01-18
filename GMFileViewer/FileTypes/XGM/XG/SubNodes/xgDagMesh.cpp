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
	bindGeometry();
	m_inputGeometry->update();
}

void xgDagMesh::draw(const DirectX::XMMATRIX& meshMatrix) const
{
	resetMaterialSlot();
	m_inputMaterial->bind();
	
	bindGeometry();
	setCulling();

	/* Update meshMatrix in const buffer
		[HERE]
	*/

	if (m_primType == 5)
	{
		m_prim.draw<5>();
		m_triFan.draw<5>();
		m_triStrip.draw<5>();
		m_triList.draw<5>();
	}
	else if (m_primType == 4)
	{
		m_prim.draw<4>();
		m_triFan.draw<4>();
		m_triStrip.draw<4>();
		m_triList.draw<4>();
	}
}

void xgDagMesh::resetMaterialSlot() const
{
	/* Set to Material & Texture slot 1
		[HERE]
	*/
}

void xgDagMesh::bindGeometry() const
{
}

void xgDagMesh::setCulling() const
{
	switch (m_cullFunc)
	{
	case 0:
		break;
	default:
		break;
	}
}
