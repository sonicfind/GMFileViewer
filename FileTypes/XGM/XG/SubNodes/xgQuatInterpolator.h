#pragma once
#include "XG_InterpolatorNodes.h"

class xgQuatInterpolator : public XG_InterpolatorNode<DirectX::XMVECTOR>
{
public:
	DirectX::XMVECTOR calcMixedValue() const;

private:
	void loadKeys(const char*& input);
};
