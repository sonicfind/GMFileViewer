#include "xgVec3Interpolator.h"

DirectX::XMFLOAT3 xgVec3Interpolator::calcMixedValue() const
{
	return getInterpolatedKey();
}

void xgVec3Interpolator::loadKeys(FilePointer& file)
{
	m_keys.reserve_and_fill(file);
}
