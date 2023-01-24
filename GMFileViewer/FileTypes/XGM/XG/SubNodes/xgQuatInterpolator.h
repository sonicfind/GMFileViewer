#pragma once
#include "XG_InterpolatorNodes.h"

class xgQuatInterpolator : public XG_InterpolatorNode<DirectX::XMVECTOR>
{
public:
	void writeType(FileWriter& file) const override;
	DirectX::XMVECTOR calcMixedValue() const;
};
