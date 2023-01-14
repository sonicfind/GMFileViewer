#pragma once
#include <DirectXMath.h>
#include "XG_InterpolatorNodes.h"

class xgBgMatrix : public XG_SubNode
{
	DirectX::XMFLOAT3 m_position;
	DirectX::XMVECTOR m_rotation;
	DirectX::XMFLOAT3 m_scale;
	xgVec3Interpolator* m_inputPosition = nullptr;
	xgQuatInterpolator* m_inputRotation = nullptr;
	xgVec3Interpolator* m_inputScale = nullptr;
	xgBgMatrix* m_inputParentMatrix = nullptr;

public:
	void load(const char*& input, const std::vector<XGVectElement>& nodes) override;

};

