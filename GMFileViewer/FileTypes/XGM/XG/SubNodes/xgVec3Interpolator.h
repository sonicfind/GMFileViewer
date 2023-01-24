#pragma once
#include "XG_InterpolatorNodes.h"

class xgVec3Interpolator : public XG_InterpolatorNode<DirectX::XMFLOAT3>
{
public:
	void writeType(FileWriter& file) const override;
	DirectX::XMFLOAT3 calcMixedValue() const;
};
