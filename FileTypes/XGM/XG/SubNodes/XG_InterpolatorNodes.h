#pragma once
#include "VertexList.h"
#include "xgTime.h"
#include "GMArray.h"

enum class InterpolatorType
{
	BASE,
	TIMED,
	TARGETED,
};

template <class T, InterpolatorType INTERPOLATION = InterpolatorType::BASE>
class XG_InterpolatorNode : public XG_SubNode
{
	static_assert(INTERPOLATION >= InterpolatorType::BASE && INTERPOLATION <= InterpolatorType::TARGETED);
protected:
	uint32_t m_type = 0;
	GMArray<float> m_times;
	GMArray<T> m_keys;
	GMArray<uint32_t> m_targets;
	xgTime* m_inputTime = nullptr;

public:
	void load(const char*& input, const std::vector<XGVectElement>& nodes) override
	{
		PString::ReadNamedValue("type", m_type, input);

		if constexpr (INTERPOLATION >= InterpolatorType::TIMED)
		{
			PString::CheckForString("times", input);
			GMArrayLoader::loadElements(m_times, input);
		}

		PString::CheckForString("keys", input);
		GMArrayLoader::loadElements(m_keys, input);

		if constexpr (INTERPOLATION == InterpolatorType::TARGETED)
		{
			PString::CheckForString("targets", input);
			GMArrayLoader::loadElements(m_targets, input);
		}

		while (BindNode_optional(m_inputTime, "inputTime", "outputTime", input, nodes));
	}
};

class xgVec3Interpolator     : public XG_InterpolatorNode<Eigen::Vector3f> {};
class xgQuatInterpolator     : public XG_InterpolatorNode<Eigen::Quaternionf> {};

class xgVertexInterpolator   : public XG_InterpolatorNode<GMArray<Eigen::Vector3f>, InterpolatorType::TARGETED> {};
class xgNormalInterpolator   : public XG_InterpolatorNode<GMArray<Eigen::Vector3f>, InterpolatorType::TARGETED> {};
class xgTexCoordInterpolator : public XG_InterpolatorNode<GMArray<Eigen::Vector2f>, InterpolatorType::TARGETED> {};

class xgShapeInterpolator    : public XG_InterpolatorNode<VertexList, InterpolatorType::TIMED> {};
