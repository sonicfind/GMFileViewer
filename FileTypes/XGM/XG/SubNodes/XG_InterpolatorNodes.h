#pragma once
#include "VertexList.h"
#include "xgTime.h"
#include "GMArray.h"
#include "../PString.h"

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
	uint32_t m_type = 0;
	GMArray<float> m_times;
	GMArray<T> m_keys;
	GMArray<uint32_t> m_targets;
	xgTime* m_inputTime = nullptr;

public:
	void load(const char*& input, const XG* xg) override
	{
		PString::ReadNamedValue("type", m_type, input);

		if constexpr (INTERPOLATION >= InterpolatorType::TIMED)
			PString::ReadNamedValue("times", m_times, input);

		PString::CheckForString("keys", input);
		loadKeys(input);

		if constexpr (INTERPOLATION == InterpolatorType::TARGETED)
		{
			PString::ReadNamedValue("targets", m_targets, input);
		}

		while (XG_SubNode* node = xg->grabNode_optional("inputTime", "outputTime", input))
			m_inputTime = static_cast<xgTime*>(node);
	}
private:

	void loadKeys(const char*& input)
	{
		m_keys.reserve_and_fill(input);
	}
};

template<>
void XG_InterpolatorNode<GMArray<DirectX::XMFLOAT3>, InterpolatorType::TARGETED>::loadKeys(const char*& input);

template<>
void XG_InterpolatorNode<GMArray<DirectX::XMFLOAT2>, InterpolatorType::TARGETED>::loadKeys(const char*& input);

template<>
void XG_InterpolatorNode<VertexList, InterpolatorType::TIMED>::loadKeys(const char*& input);

class xgVec3Interpolator     : public XG_InterpolatorNode<DirectX::XMFLOAT3> {};
class xgQuatInterpolator     : public XG_InterpolatorNode<DirectX::XMVECTOR> {};

class xgVertexInterpolator   : public XG_InterpolatorNode<GMArray<DirectX::XMFLOAT3>, InterpolatorType::TARGETED> {};
class xgNormalInterpolator   : public XG_InterpolatorNode<GMArray<DirectX::XMFLOAT3>, InterpolatorType::TARGETED> {};
class xgTexCoordInterpolator : public XG_InterpolatorNode<GMArray<DirectX::XMFLOAT2>, InterpolatorType::TARGETED> {};

class xgShapeInterpolator    : public XG_InterpolatorNode<VertexList, InterpolatorType::TIMED> {};
