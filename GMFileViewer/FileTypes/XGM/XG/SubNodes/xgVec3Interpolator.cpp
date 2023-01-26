#include "xgVec3Interpolator.h"

void xgVec3Interpolator::writeType(FileWriter& file) const
{
	PString::WriteString("xgVec3Interpolator", file);
}

glm::vec3 xgVec3Interpolator::calcMixedValue() const
{
	return getInterpolatedKey();
}

glm::vec3 xgVec3Interpolator::mixKeys(const glm::vec3& first, const glm::vec3& second, float coef) const
{
	return glm::mix(first, second, coef);
}
