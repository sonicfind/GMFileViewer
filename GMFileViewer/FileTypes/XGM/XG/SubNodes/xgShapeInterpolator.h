#pragma once
#include "XG_InterpolatorNodes.h"
#include "XG_UpdatableNodes.h"

class xgShapeInterpolator : public XG_InterpolatorNode<VertexList, InterpolatorType::TIMED>, public XG_UpdatableNode
{
public:
	void updateVertices(VertexList& vertices) const override;

private:
	void loadKeys(FilePointer& file) override;
};
