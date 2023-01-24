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

void xgDagMesh::writeType(FileWriter& file) const
{
	PString::WriteString("xgDagMesh", file);
}

void xgDagMesh::save(FileWriter& file, const XG* xg) const
{
	PString::WriteNamedValue("primType", m_primType, file);

	m_prim.save(file);
	m_triFan.save(file);
	m_triStrip.save(file);
	m_triList.save(file);

	PString::WriteNamedValue("cullFunc", m_cullFunc, file);

	xg->writeNode("inputGeometry", "outputGeometry", m_inputGeometry, file);
	if (m_inputMaterial)
		xg->writeNode("inputMaterial", "outputMaterial", reinterpret_cast<XG_SubNode*>(m_inputMaterial), file);
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
	Graphics* gfx = Graphics::getGraphics();
	gfx->bindConstantBuffer(Graphics::ModelMatrix);
	gfx->updateConstantBuffer(0, &meshMatrix, sizeof(DirectX::XMMATRIX));

	m_inputGeometry->bindVertexBuffer();

	gfx->setCullFunc(m_cullFunc);
	gfx->activateShader(Graphics::Model);

	gfx->setActiveTexture(0);
	gfx->setDepthFunc(DepthTest::LESS);
	for (uint32_t i = 0; i < m_inputMaterial->getNumMaterials(); ++i)
	{
		m_inputMaterial->bind(i);
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
