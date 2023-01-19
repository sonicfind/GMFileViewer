#pragma once
#include "XG_InterpolatorNodes.h"
#include "XG_UpdatableNodes.h"
#include "Graphics.h"

template <typename T, size_t offset>
class XG_TargetedInterpolator : public XG_InterpolatorNode<GMArray<T>, InterpolatorType::TARGETED>, public XG_UpdatableNode
{
	using XG_InterpolatorNode<GMArray<T>, InterpolatorType::TARGETED>::m_keys;
	using XG_InterpolatorNode<GMArray<T>, InterpolatorType::TARGETED>::m_targets;
public:
	void updateVertexBuffer() const override
	{
		const Graphics* gfx = Graphics::getGraphics();
		const GMArray<T> interpolatedKeys = XG_InterpolatorNode<GMArray<T>, InterpolatorType::TARGETED>::getInterpolatedKey();
		for (uint32_t targetIndex = 0; targetIndex < m_targets.getSize(); ++targetIndex)
			gfx->updateVertexBuffer(m_targets[targetIndex] * sizeof(Vertex) + offset, &interpolatedKeys[targetIndex], sizeof(T));
	}

private:
	void loadKeys(FilePointer& file) override
	{
		if (m_keys.reserve(file))
			for (auto& element : m_keys)
				element.reserve_and_fill(file);
	}
};

using xgVertexInterpolator = XG_TargetedInterpolator<DirectX::XMFLOAT3, 0>;
using xgNormalInterpolator = XG_TargetedInterpolator<DirectX::XMFLOAT3, sizeof(DirectX::XMVECTOR)>;
using xgTexCoordInterpolator = XG_TargetedInterpolator<DirectX::XMFLOAT2, 2 * sizeof(DirectX::XMVECTOR) + sizeof(DirectX::XMFLOAT3)>;
