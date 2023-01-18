#pragma once
#include "XG_InterpolatorNodes.h"

class xgVec3Interpolator : public XG_InterpolatorNode<DirectX::XMFLOAT3>
{
public:
	DirectX::XMFLOAT3 calcMixedValue() const;

private:
	void loadKeys(FilePointer& file);
};
