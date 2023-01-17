#pragma once
#include "XG_InterpolatorNodes.h"
#include "XG_UpdatableNodes.h"

class xgVertexInterpolator : public XG_InterpolatorNode<GMArray<DirectX::XMFLOAT3>, InterpolatorType::TARGETED>, public XG_UpdatableNode
{
public:
	void updateVertexBuffer() const override;

private:
	void loadKeys(const char*& input) override;
};
