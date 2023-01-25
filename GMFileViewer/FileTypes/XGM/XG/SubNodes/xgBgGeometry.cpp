#include "xgBgGeometry.h"
#include "../PString.h"
#include "FilePointer.h"

void xgBgGeometry::load(FilePointer& file, const XG* xg)
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

void xgBgGeometry::save(FileWriter& file, const XG* xg) const
{
	PString::WriteNamedValue("density", m_density, file);

	PString::WriteString("vertices", file);
	m_vertices.save(file);

	for (const auto node : m_inputGeometries)
		xg->writeNode("inputGeometry", "outputGeometry", dynamic_cast<XG_SubNode*>(node), file);

}

void xgBgGeometry::createVertexBuffer()
{
	m_vertices.createVertexBuffer(!m_inputGeometries.empty());
}

void xgBgGeometry::bindVertexBuffer() const
{
	m_vertices.bindBuffer();
}

void xgBgGeometry::update()
{
	if (m_inputGeometries.empty())
		return;

	for (const XG_UpdatableNode* node : m_inputGeometries)
		node->updateVertices(m_vertices);

	m_vertices.bindBuffer();
	m_vertices.updateBuffer();
}

const Vertex& xgBgGeometry::getVertex(size_t index) const
{
	return m_vertices[index];
}
