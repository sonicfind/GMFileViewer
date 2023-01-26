#pragma once
#include "XG_InterpolatorNodes.h"
#include "XG_UpdatableNodes.h"

class xgShapeInterpolator : public XG_InterpolatorNode<VertexList, InterpolatorType::TIMED>, public XG_UpdatableNode
{
public:
	using XG_InterpolatorNode::XG_InterpolatorNode;
	void writeType(FileWriter& file) const override;
	void updateVertices(VertexList& vertices) const override;

private:
	void loadKeys(FileReader& file) override;
	void saveKeys(FileWriter& file) const override;
	VertexList mixKeys(const VertexList& first, const VertexList& second, float coef) const override;
};
