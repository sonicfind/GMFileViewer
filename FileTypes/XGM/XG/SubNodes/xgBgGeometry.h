#pragma once
#include "../XG.h"
#include "XG_UpdatableNodes.h"
#include "VertexList.h"

class xgBgGeometry : public XG_SubNode
{
	float m_density = 0;
	VertexList m_vertices;
	std::vector<XG_UpdatableNode*> m_inputGeometries;

public:
	void load(FilePointer& file, const XG* xg) override;
	void update() const;

	const Vertex& getVertex(size_t index) const;
};
