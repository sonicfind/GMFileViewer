#include "xgQuatInterpolator.h"

void xgQuatInterpolator::writeType(FileWriter& file) const
{
	PString::WriteString("xgQuatInterpolator", file);
}

glm::quat xgQuatInterpolator::calcMixedValue() const
{
	return getInterpolatedKey();
}

glm::quat xgQuatInterpolator::mixKeys(const glm::quat& first, const glm::quat& second, float coef) const
{
	return glm::slerp(first, second, coef);
}
