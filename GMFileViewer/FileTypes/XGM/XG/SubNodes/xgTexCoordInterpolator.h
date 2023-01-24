#pragma once
#include "XG_TargetedInterpolators.h"
#include "XG_UpdatableNodes.h"

class xgTexCoordInterpolator : public XG_TargetedInterpolator<DirectX::XMFLOAT2>, public XG_UpdatableNode
{
public:
	void writeType(FileWriter& file) const override;
	void updateVertices(VertexList& vertices) const override;
};
