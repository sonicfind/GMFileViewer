#pragma once
#include "XG_InterpolatorNodes.h"
#include "XG_UpdatableNodes.h"

class xgTexCoordInterpolator : public XG_InterpolatorNode<GMArray<DirectX::XMFLOAT2>, InterpolatorType::TARGETED>, public XG_UpdatableNode
{
public:
	void updateVertexBuffer() const override;

private:
	void loadKeys(FilePointer& file) override;
};
