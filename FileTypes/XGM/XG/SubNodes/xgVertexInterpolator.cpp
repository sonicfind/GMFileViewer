#include "xgVertexInterpolator.h"

void xgVertexInterpolator::updateVertexBuffer() const
{
	const GMArray<DirectX::XMFLOAT3> interpolatedKeys = getInterpolatedKey();
	for (uint32_t targetIndex = 0; targetIndex < m_targets.getSize(); ++targetIndex)
	{
		// Use applicable "SubData" commands here w/ memory offset of 0
	}
}

void xgVertexInterpolator::loadKeys(const char*& input)
{
	if (m_keys.reserve(input))
		for (auto& element : m_keys)
			element.reserve_and_fill(input);
}
