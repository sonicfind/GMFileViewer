#include "xgQuatInterpolator.h"

DirectX::XMVECTOR xgQuatInterpolator::calcMixedValue() const
{
	return getInterpolatedKey();
}

void xgQuatInterpolator::loadKeys(FilePointer& file)
{
	m_keys.reserve_and_fill(file);
}
