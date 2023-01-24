#include "xgQuatInterpolator.h"

void xgQuatInterpolator::writeType(FileWriter& file) const
{
	PString::WriteString("xgQuatInterpolator", file);
}

DirectX::XMVECTOR xgQuatInterpolator::calcMixedValue() const
{
	return getInterpolatedKey();
}
