#include "xgVec3Interpolator.h"

void xgVec3Interpolator::writeType(FileWriter& file) const
{
	PString::WriteString("xgVec3Interpolator", file);
}

DirectX::XMFLOAT3 xgVec3Interpolator::calcMixedValue() const
{
	return getInterpolatedKey();
}
