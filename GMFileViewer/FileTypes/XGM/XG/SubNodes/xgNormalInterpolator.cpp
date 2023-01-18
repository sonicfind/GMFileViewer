#include "xgNormalInterpolator.h"

void xgNormalInterpolator::updateVertexBuffer() const
{
	const GMArray<DirectX::XMFLOAT3> interpolatedKeys = getInterpolatedKey();
	for (uint32_t targetIndex = 0; targetIndex < m_targets.getSize(); ++targetIndex)
	{
		// Use applicable "SubData" commands here w/ memory offset of `sizeof(DirectX::XMVECTOR)`
	}
}

void xgNormalInterpolator::loadKeys(FilePointer& file)
{
	if (m_keys.reserve(file))
		for (auto& element : m_keys)
			element.reserve_and_fill(file);
}