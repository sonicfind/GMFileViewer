#pragma once
#include "XG_TargetedInterpolators.h"
#include "XG_UpdatableNodes.h"

class xgTexCoordInterpolator : public XG_TargetedInterpolator<DirectX::XMFLOAT2>, public XG_UpdatableNode
{
public:
	void updateVertices(VertexList& vertices) const override;
};
