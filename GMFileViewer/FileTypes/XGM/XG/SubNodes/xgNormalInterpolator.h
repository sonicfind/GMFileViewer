#pragma once
#include "XG_TargetedInterpolators.h"
#include "XG_UpdatableNodes.h"

class xgNormalInterpolator : public XG_TargetedInterpolator<glm::vec3>, public XG_UpdatableNode
{
public:
	using XG_TargetedInterpolator::XG_TargetedInterpolator;
	void writeType(FileWriter& file) const override;
	void updateVertices(VertexList& vertices) const override;
};
