#include "xgTexCoordInterpolator.h"

void xgTexCoordInterpolator::updateVertexBuffer() const
{
	const GMArray<DirectX::XMFLOAT2> interpolatedKeys = getInterpolatedKey();
	for (uint32_t targetIndex = 0; targetIndex < m_targets.getSize(); ++targetIndex)
	{
		// Use applicable "SubData" commands here w/ memory offset of `2 * sizeof(DirectX::XMVECTOR) + sizeof(DirectX::XMFLOAT3)`
	}
}

void xgTexCoordInterpolator::loadKeys(FilePointer& file)
{
	if (m_keys.reserve(file))
		for (auto& element : m_keys)
			element.reserve_and_fill(file);
}
