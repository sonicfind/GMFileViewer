#include "xgBgGeometry.h"
#include "../PString.h"
#include "FileReader.h"

void xgBgGeometry::load(FileReader& file, const XG* xg)
{
	PString::ReadNamedValue("density", m_density, file);

	PString::ThrowOnStringMismatch("vertices", file);
	m_vertices.load(file);

	while (XG_SubNode* node = xg->grabNode_optional("inputGeometry", "outputGeometry", file))
	{
		XG_UpdatableNode* updatable = dynamic_cast<XG_UpdatableNode*>(node);
		if (!updatable)
			throw "Aight, come on mate";
		m_inputGeometries.push_back(updatable);	
	}
}

void xgBgGeometry::writeType(FileWriter& file) const
{
	PString::WriteString("xgBgGeometry", file);
}

void xgBgGeometry::save(FileWriter& file) const
{
	PString::WriteNamedValue("density", m_density, file);

	PString::WriteString("vertices", file);
	m_vertices.save(file);

	for (const auto node : m_inputGeometries)
		WriteNode("inputGeometry", "outputGeometry", dynamic_cast<XG_SubNode*>(node), file);

}

void xgBgGeometry::createVertexBuffer(Graphics& gfx)
{
	m_vertices.createVertexBuffer(gfx, !m_inputGeometries.empty());
}

void xgBgGeometry::addInstance(Graphics& gfx)
{
	m_vertices.addInstance(gfx, !m_inputGeometries.empty());
}

void xgBgGeometry::bindVertexBuffer(Graphics& gfx, uint32_t instance) const
{
	m_vertices.bindBuffer(gfx, instance);
}

void xgBgGeometry::update(Graphics& gfx, uint32_t instance)
{
	if (m_inputGeometries.empty())
		return;

	for (const XG_UpdatableNode* node : m_inputGeometries)
		node->updateVertices(m_vertices);

	m_vertices.bindBuffer(gfx, instance);
	m_vertices.updateBuffer(gfx);
}

const Vertex& xgBgGeometry::getVertex(size_t index) const
{
	return m_vertices[index];
}
