#include "xgQuatInterpolator.h"

DirectX::XMVECTOR xgQuatInterpolator::calcMixedValue() const
{
	return getInterpolatedKey();
}

void xgQuatInterpolator::loadKeys(const char*& input)
{
	m_keys.reserve_and_fill(input);
}
