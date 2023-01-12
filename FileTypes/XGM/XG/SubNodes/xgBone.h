#pragma once
#include "xgBgMatrix.h"
#include <glm/glm/glm.hpp>

class xgBone : public XG_SubNode
{
	glm::mat4 m_restMatrix;
	xgBgMatrix* m_inputMatrix = nullptr;

public:
	void load(const char*& input, const std::vector<XGVectElement>& nodes) override;
};

