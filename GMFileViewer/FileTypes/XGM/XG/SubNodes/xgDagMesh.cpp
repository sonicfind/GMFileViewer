#include "xgDagMesh.h"

xgDagMesh::~xgDagMesh()
{
}

void xgDagMesh::load(FileReader& file, const XG* xg)
{
	PString::ReadNamedValue("primType", m_primType, file);

	if (m_primType == 4)
	{
		m_prim = new Triangle_Separate<PrimitiveMode::PRIMITIVE>(file);
		m_triFan = new Triangle_Separate<PrimitiveMode::TRIANGLE_FAN>(file);
		m_triStrip = new Triangle_Separate<PrimitiveMode::TRIANGLE_STRIP>(file);
		m_triList = new Triangle_Separate<PrimitiveMode::TRIANGLE_LIST>(file);
	}
	else
	{
		m_prim = new Triangle_Group<PrimitiveMode::PRIMITIVE>(file);
		m_triFan = new Triangle_Group<PrimitiveMode::TRIANGLE_FAN>(file);
		m_triStrip = new Triangle_Group<PrimitiveMode::TRIANGLE_STRIP>(file);
		m_triList = new Triangle_Group<PrimitiveMode::TRIANGLE_LIST>(file);
	}
	

	PString::ReadNamedValue("cullFunc", m_cullFunc, file);

	while (xg->grabNode_nondestructive(m_inputGeometry, "inputGeometry", "outputGeometry", file) ||
		   xg->grabNode_nondestructive(m_inputMaterial, "inputMaterial", "outputMaterial", file));
}

void xgDagMesh::writeType(FileWriter& file) const
{
	PString::WriteString("xgDagMesh", file);
}

void xgDagMesh::save(FileWriter& file) const
{
	PString::WriteNamedValue("primType", m_primType, file);

	m_prim->save(file);
	m_triFan->save(file);
	m_triStrip->save(file);
	m_triList->save(file);

	PString::WriteNamedValue("cullFunc", m_cullFunc, file);

	WriteNode("inputGeometry", "outputGeometry", m_inputGeometry, file);
	if (m_inputMaterial)
		WriteNode("inputMaterial", "outputMaterial", m_inputMaterial, file);
}

bool xgDagMesh::hasTransparency() const
{
	return m_inputMaterial != nullptr && m_inputMaterial->hasTransparency();
}

void xgDagMesh::createVertexBuffer(Graphics& gfx)
{
	m_inputGeometry->createVertexBuffer(gfx);
	if (m_primType == 4)
	{
		reinterpret_cast<Triangle_Separate<PrimitiveMode::TRIANGLE_FAN>*>(m_triFan)->CreateIndexBuffer(gfx);
		reinterpret_cast<Triangle_Separate<PrimitiveMode::TRIANGLE_STRIP>*>(m_triStrip)->CreateIndexBuffer(gfx);
		reinterpret_cast<Triangle_Separate<PrimitiveMode::TRIANGLE_LIST>*>(m_triList)->CreateIndexBuffer(gfx);
	}
}

void xgDagMesh::addInstance(Graphics& gfx)
{
	m_inputGeometry->addInstance(gfx);
}

void xgDagMesh::update(Graphics& gfx, uint32_t instance)
{
	m_inputGeometry->update(gfx, instance);
}

void xgDagMesh::draw(Graphics& gfx, uint32_t instance, const glm::mat4& meshMatrix) const
{
	gfx.bindConstantBuffer(Graphics::ModelMatrix);
	gfx.updateConstantBuffer(0, &meshMatrix, sizeof(glm::mat4));
	const glm::mat4 normalMatrix = glm::transpose(glm::inverse(meshMatrix));
	gfx.updateConstantBuffer(sizeof(glm::mat4), &normalMatrix, sizeof(glm::mat4));

	m_inputGeometry->bindVertexBuffer(gfx, instance);

	gfx.setCullFunc(m_cullFunc);
	gfx.setActiveTexture(0);
	gfx.setDepthFunc(DepthTest::LESS);
	for (uint32_t i = 0; i < m_inputMaterial->getNumMaterials(); ++i)
	{
		m_inputMaterial->bind(gfx, i);
		m_prim->draw(gfx);
		m_triFan->draw(gfx);
		m_triStrip->draw(gfx);
		m_triList->draw(gfx);
		gfx.setDepthFunc(DepthTest::L_EQUAL);
	}
}
