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
	using XG_SubNode::XG_SubNode;
	void load(FileReader& file, const XG* xg) override;

	void writeType(FileWriter& file) const override;
	void save(FileWriter& file) const override;

	void createVertexBuffer();
	void addInstance();
	void bindVertexBuffer(uint32_t instance) const;
	void update(uint32_t instance);

	const Vertex& getVertex(size_t index) const;
};
