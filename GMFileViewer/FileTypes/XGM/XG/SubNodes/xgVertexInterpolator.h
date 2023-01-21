#pragma once
#include "XG_TargetedInterpolators.h"
#include "XG_UpdatableNodes.h"

class xgVertexInterpolator : public XG_TargetedInterpolator<DirectX::XMFLOAT3>, public XG_UpdatableNode
{
public:
	void updateVertices(VertexList& vertices) const override;
};
