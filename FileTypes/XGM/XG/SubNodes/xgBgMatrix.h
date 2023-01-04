#pragma once
#include <Eigen/Dense>
#include "XG_InterpolatorNodes.h"

class xgBgMatrix : public XG_SubNode
{
	Eigen::Vector3f m_position;
	Eigen::Quaternionf m_rotation;
	Eigen::Vector3f m_scale;
	xgVec3Interpolator* m_inputPosition = nullptr;
	xgQuatInterpolator* m_inputRotation = nullptr;
	xgVec3Interpolator* m_inputScale = nullptr;
	xgBgMatrix* m_inputParentMatrix = nullptr;

public:
	void load(const char*& input, const std::vector<XGVectElement>& nodes) override;

};

