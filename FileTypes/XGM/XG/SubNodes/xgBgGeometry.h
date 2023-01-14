#pragma once
#include "VertexList.h"
#include "xgEnvelope.h"
#include "XG_InterpolatorNodes.h"

class xgBgGeometry : public XG_SubNode
{
	float m_density = 0;
	VertexList m_vertices;
	std::vector<XG_SubNode*> m_inputGeometries;

public:
	void load(const char*& input, const XG* xg) override;
};
