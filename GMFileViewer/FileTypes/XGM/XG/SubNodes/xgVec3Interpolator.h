#pragma once
#include "XG_InterpolatorNodes.h"

class xgVec3Interpolator : public XG_InterpolatorNode<glm::vec3>
{
public:
	using XG_InterpolatorNode::XG_InterpolatorNode;
	void writeType(FileWriter& file) const override;
	glm::vec3 calcMixedValue() const;

private:
	glm::vec3 mixKeys(const glm::vec3& first, const glm::vec3& second, float coef) const override;
};
