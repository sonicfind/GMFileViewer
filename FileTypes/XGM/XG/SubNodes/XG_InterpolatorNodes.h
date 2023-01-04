#pragma once
#include "VertexList.h"
#include "xgTime.h"
#include "XGArray.h"

template <class T, bool useTIMES = false, bool useTARGETS = false>
class XG_InterpolatorNode : public XG_SubNode
{
protected:
	uint32_t m_type = 0;
	XGArray<float> m_times;
	XGArray<T> m_keys;
	XGArray<uint32_t> m_targets;
	xgTime* m_inputTime = nullptr;

public:
	void load(const char*& input, const std::vector<XGVectElement>& nodes) override
	{
		PString::ReadNamedValue("type", m_type, input);

		if constexpr (useTIMES)
		{
			PString::CheckForString("times", input);
			XGArrayLoader::loadElements(m_times, input);
		}

		PString::CheckForString("keys", input);
		XGArrayLoader::loadElements(m_keys, input);

		if constexpr (useTARGETS)
		{
			PString::CheckForString("targets", input);
			XGArrayLoader::loadElements(m_targets, input);
		}

		while (BindNode_optional(m_inputTime, "inputTime", "outputTime", input, nodes));
	}
};

class xgVec3Interpolator     : public XG_InterpolatorNode<Eigen::Vector3f> {};
class xgQuatInterpolator     : public XG_InterpolatorNode<Eigen::Quaternionf> {};

class xgVertexInterpolator   : public XG_InterpolatorNode<XGArray<Eigen::Vector3f>, true, true> {};
class xgNormalInterpolator   : public XG_InterpolatorNode<XGArray<Eigen::Vector3f>, true, true> {};
class xgTexCoordInterpolator : public XG_InterpolatorNode<XGArray<Eigen::Vector2f>, true, true> {};

class xgShapeInterpolator    : public XG_InterpolatorNode<VertexList, true> {};
