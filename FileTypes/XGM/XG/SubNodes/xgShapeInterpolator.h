#pragma once
#include "XG_InterpolatorNodes.h"
#include "XG_UpdatableNodes.h"

class xgShapeInterpolator : public XG_InterpolatorNode<VertexList, InterpolatorType::TIMED>, public XG_UpdatableNode
{
public:
	void updateVertexBuffer() const override;

private:
	void loadKeys(const char*& input) override;
};
