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

enum class InterpolationMode : uint32_t
{
	FLAT,
	SMOOTH
};

template <class T, InterpolatorType INTERPOLATION = InterpolatorType::BASE>
class XG_InterpolatorNode : public XG_SubNode
{
	static_assert(INTERPOLATION >= InterpolatorType::BASE && INTERPOLATION <= InterpolatorType::TARGETED);
	struct Frame
	{
		float first;
		float second;
		float coef;
	};
	
	InterpolationMode m_type = InterpolationMode::FLAT;
	GMArray<float> m_times;
	GMArray<T> m_keys;
	GMArray<uint32_t> m_targets;
	xgTime* m_inputTime = nullptr;

	bool m_useTargets = false;

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

			for (unsigned long targetIndex = 0; targetIndex < m_targets.getSize(); ++targetIndex)
				if (m_targets[targetIndex] != targetIndex)
				{
					m_useTargets = true;
					break;
				}
		}

		while (XG_SubNode* node = xg->grabNode_optional("inputTime", "outputTime", input))
			m_inputTime = static_cast<xgTime*>(node);
	}

	T update()
	{
		return mapInterpolationToTargets(interpolate(calculateTimeFrame()));
	}

private:

	void loadKeys(const char*& input)
	{
		m_keys.reserve_and_fill(input);
	}

	Frame calculateTimeFrame()
	{
		float time = m_inputTime->getTime();
		if constexpr (INTERPOLATION < InterpolatorType::TIMED)
		{
			if (time + 1 >= m_keys.getSize())
				return { time, time, 0 };
			return { time, time + 1, time - uint32_t(time) };
		}
		else if (uint32_t(time) >= m_times.getSize() - 1)
		{
			time = m_times.back();
			return { time, time + 1, time - uint32_t(time) };
		}
		else
		{
			const size_t index = size_t(time);
			return { m_times[index], m_times[index + 1], time - uint32_t(time) };
		}
	}

	T getKeyPoint(const float time)
	{
		size_t index = size_t(time);
		if (index + 1 >= m_keys.getSize())
			return m_keys.back();
		else if (time == index)
			return m_keys[index];
		else
			return mixKeys(m_keys[index], m_keys[index + 1], time - index);
	}

	T interpolate(const Frame frame)
	{
		if (m_type == InterpolationMode::FLAT)
		{
			size_t index = size_t(frame.first);
			if (index >= m_keys.getSize())
				return m_keys.back();
			else
				return m_keys[index];
		}
		else if (frame.first == frame.second || frame.coef == 0)
			return getKeyPoint(frame.first);
		else
			return mixKeys(getKeyPoint(frame.first), getKeyPoint(frame.second), frame.coef);
	}

	T mapInterpolationToTargets(T interpolatedKeys)
	{
		if constexpr (INTERPOLATION == InterpolatorType::TARGETED)
		{
			if (m_useTargets)
			{
				// All official "Targeted" interpolators use arrays as keys
				T result(interpolatedKeys.getSize());
				for (uint32_t targetIndex = 0; targetIndex < m_targets.getSize(); ++targetIndex)
					result[m_targets[targetIndex]] = interpolatedKeys[targetIndex];
				return result;
			}
		}
		return interpolatedKeys;
	}

	static T mixKeys(const T& first, const T& second, const float coef)
	{
		return first + (second - first) * coef;
	}
};

template<>
void XG_InterpolatorNode<GMArray<DirectX::XMFLOAT3>, InterpolatorType::TARGETED>::loadKeys(const char*& input);

template<>
void XG_InterpolatorNode<GMArray<DirectX::XMFLOAT2>, InterpolatorType::TARGETED>::loadKeys(const char*& input);

template<>
void XG_InterpolatorNode<VertexList, InterpolatorType::TIMED>::loadKeys(const char*& input);

template<>
DirectX::XMFLOAT3 XG_InterpolatorNode<DirectX::XMFLOAT3>::mixKeys(const DirectX::XMFLOAT3& first, const DirectX::XMFLOAT3& second, const float coef);

template<>
DirectX::XMVECTOR XG_InterpolatorNode<DirectX::XMVECTOR>::mixKeys(const DirectX::XMVECTOR& first, const DirectX::XMVECTOR& second, const float coef);

template<>
GMArray<DirectX::XMFLOAT3> XG_InterpolatorNode<GMArray<DirectX::XMFLOAT3>, InterpolatorType::TARGETED>::mixKeys(const GMArray<DirectX::XMFLOAT3>& first, const GMArray<DirectX::XMFLOAT3>& second, const float coef);

template<>
GMArray<DirectX::XMFLOAT2> XG_InterpolatorNode<GMArray<DirectX::XMFLOAT2>, InterpolatorType::TARGETED>::mixKeys(const GMArray<DirectX::XMFLOAT2>& first, const GMArray<DirectX::XMFLOAT2>& second, const float coef);

template<>
VertexList XG_InterpolatorNode<VertexList, InterpolatorType::TIMED>::mixKeys(const VertexList& first, const VertexList& second, const float coef);

class xgVec3Interpolator     : public XG_InterpolatorNode<DirectX::XMFLOAT3> {};
class xgQuatInterpolator     : public XG_InterpolatorNode<DirectX::XMVECTOR> {};

class xgVertexInterpolator   : public XG_InterpolatorNode<GMArray<DirectX::XMFLOAT3>, InterpolatorType::TARGETED> {};
class xgNormalInterpolator   : public XG_InterpolatorNode<GMArray<DirectX::XMFLOAT3>, InterpolatorType::TARGETED> {};
class xgTexCoordInterpolator : public XG_InterpolatorNode<GMArray<DirectX::XMFLOAT2>, InterpolatorType::TARGETED> {};

class xgShapeInterpolator    : public XG_InterpolatorNode<VertexList, InterpolatorType::TIMED> {};
