#pragma once
#include "XG_InterpolatorNodes.h"
#include <glm/gtx/quaternion.hpp>

class xgQuatInterpolator : public XG_InterpolatorNode<glm::quat>
{
public:
	using XG_InterpolatorNode::XG_InterpolatorNode;
	void writeType(FileWriter& file) const override;
	glm::quat calcMixedValue() const;

private:
	glm::quat mixKeys(const glm::quat& first, const glm::quat& second, float coef) const override;
};
