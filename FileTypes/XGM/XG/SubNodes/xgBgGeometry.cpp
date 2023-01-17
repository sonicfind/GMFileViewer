#include "xgBgGeometry.h"
#include "../PString.h"
#include "FileOperations.h"

void xgBgGeometry::load(const char*& input, const XG* xg)
{
	PString::ReadNamedValue("density", m_density, input);
	PString::ReadNamedValue("vertices", m_vertices, input);

	while (XG_SubNode* node = xg->grabNode_optional("inputGeometry", "outputGeometry", input))
	{
		XG_UpdatableNode* updatable = dynamic_cast<XG_UpdatableNode*>(node);
		if (!updatable)
			throw "Aight, come on mate";
		m_inputGeometries.push_back(updatable);	
	}
}

void xgBgGeometry::update() const
{
	// Bind vertex buffer

	for (const XG_UpdatableNode* node : m_inputGeometries)
		node->updateVertexBuffer();

	// Unbind vertex buffer
}

const Vertex& xgBgGeometry::getVertex(size_t index) const
{
	return m_vertices[index];
}
