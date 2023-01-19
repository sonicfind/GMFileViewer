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

	struct TransformVectors
	{
		DirectX::XMVECTOR translation;
		DirectX::XMVECTOR rotation;
		DirectX::XMVECTOR scale;

		DirectX::XMMATRIX getMatrix() const;
	};

public:
	void load(FilePointer& file, const XG* xg) override;

	TransformVectors transform() const;
};

