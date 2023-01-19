#include "xgBgGeometry.h"
#include "../PString.h"
#include "FilePointer.h"

void xgBgGeometry::load(FilePointer& file, const XG* xg)
{
	PString::ReadNamedValue("density", m_density, file);
	PString::ReadNamedValue("vertices", m_vertices, file);

	while (XG_SubNode* node = xg->grabNode_optional("inputGeometry", "outputGeometry", file))
	{
		XG_UpdatableNode* updatable = dynamic_cast<XG_UpdatableNode*>(node);
		if (!updatable)
			throw "Aight, come on mate";
		m_inputGeometries.push_back(updatable);	
	}
}

void xgBgGeometry::createVertexBuffer()
{
	m_vertices.createVertexBuffer(!m_inputGeometries.empty());
}

void xgBgGeometry::bindVertexBuffer() const
{
	m_vertices.bindBuffer();
}

void xgBgGeometry::update() const
{
	for (const XG_UpdatableNode* node : m_inputGeometries)
		node->updateVertexBuffer();
}

const Vertex& xgBgGeometry::getVertex(size_t index) const
{
	return m_vertices[index];
}
