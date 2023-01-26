#pragma once
#include "xgVec3Interpolator.h"
#include "xgQuatInterpolator.h"

class xgBgMatrix : public XG_SubNode
{
	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_scale;
	xgVec3Interpolator* m_inputPosition = nullptr;
	xgQuatInterpolator* m_inputRotation = nullptr;
	xgVec3Interpolator* m_inputScale = nullptr;
	xgBgMatrix* m_inputParentMatrix = nullptr;

public:
	using XG_SubNode::XG_SubNode;
	void load(FileReader& file, const XG* xg) override;

	void writeType(FileWriter& file) const override;
	void save(FileWriter& file) const override;

	glm::mat4 transform() const;

private:
	struct Transformations
	{
		glm::vec3 translation{ 0 };
		glm::quat rotation{ 1, 0, 0, 0 };
		glm::vec3 scale{ 1.f };
	};
	Transformations gatherTransformations() const;
};

