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

void xgDagMesh::createVertexBuffer()
{
	m_inputGeometry->createVertexBuffer();
}

void xgDagMesh::update()
{
	m_inputGeometry->update();
}

void xgDagMesh::draw(const DirectX::XMMATRIX& meshMatrix) const
{
	m_inputMaterial->bind(0);
	m_inputGeometry->bindVertexBuffer();

	Graphics* gfx = Graphics::getGraphics();
	gfx->bindConstantBuffer(Graphics::MeshMatrix);
	gfx->updateConstantBuffer(0, &meshMatrix, sizeof(DirectX::XMMATRIX));

	gfx->setCullFunc(m_cullFunc);
	gfx->setDepthFunc(DepthTest::LESS);

	for (size_t i = 0; i < m_inputMaterial->getNumMaterials(); ++i)
	{
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

		gfx->setDepthFunc(DepthTest::EQUAL);
	}
}
