#pragma once
#include "xgBgMatrix.h"
#include <Eigen/Dense>

class xgBone : public XG_SubNode
{
	Eigen::Matrix4f m_restMatrix;
	xgBgMatrix* m_inputMatrix = nullptr;

public:
	void load(const char*& input, const std::vector<XGVectElement>& nodes) override;
};

