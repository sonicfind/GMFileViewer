#pragma once
#include <DirectXMath.h>
#include "xgVec3Interpolator.h"
#include "xgQuatInterpolator.h"

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
	using XG_SubNode::XG_SubNode;
	void load(FileReader& file, const XG* xg) override;

	void writeType(FileWriter& file) const override;
	void save(FileWriter& file) const override;

	DirectX::XMMATRIX transform() const;
};

