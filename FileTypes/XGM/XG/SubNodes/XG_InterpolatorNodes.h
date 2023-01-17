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
	enum class InterpolationMode : uint32_t
	{
		FLAT,
		SMOOTH
	};

protected:
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

public:
	void load(const char*& input, const XG* xg) override
	{
		PString::ReadNamedValue("type", m_type, input);

		if constexpr (INTERPOLATION >= InterpolatorType::TIMED)
			PString::ReadNamedValue("times", m_times, input);

		PString::CheckForString("keys", input);
		loadKeys(input);

		if constexpr (INTERPOLATION == InterpolatorType::TARGETED)
			PString::ReadNamedValue("targets", m_targets, input);

		while (XG_SubNode* node = xg->grabNode_optional("inputTime", "outputTime", input))
			m_inputTime = static_cast<xgTime*>(node);
	}

protected:
	T getInterpolatedKey() const
	{
		return interpolateFrame(calculateTimeFrame());
	}

	virtual void loadKeys(const char*& input) = 0;
private:
	Frame calculateTimeFrame() const
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

	T getKeyPoint(const float time) const
	{
		size_t index = size_t(time);
		if (index + 1 >= m_keys.getSize())
			return m_keys.back();
		else if (time == index)
			return m_keys[index];
		else
			return MixKeys(m_keys[index], m_keys[index + 1], time - index);
	}

	T interpolateFrame(const Frame frame) const
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
			return MixKeys(getKeyPoint(frame.first), getKeyPoint(frame.second), frame.coef);
	}

	static T MixKeys(const T& first, const T& second, const float coef)
	{
		return first + (second - first) * coef;
	}
};

template<>
DirectX::XMFLOAT3 XG_InterpolatorNode<DirectX::XMFLOAT3>::MixKeys(const DirectX::XMFLOAT3& first, const DirectX::XMFLOAT3& second, const float coef);

template<>
DirectX::XMVECTOR XG_InterpolatorNode<DirectX::XMVECTOR>::MixKeys(const DirectX::XMVECTOR& first, const DirectX::XMVECTOR& second, const float coef);

template<>
GMArray<DirectX::XMFLOAT3> XG_InterpolatorNode<GMArray<DirectX::XMFLOAT3>, InterpolatorType::TARGETED>::MixKeys(const GMArray<DirectX::XMFLOAT3>& first, const GMArray<DirectX::XMFLOAT3>& second, const float coef);

template<>
GMArray<DirectX::XMFLOAT2> XG_InterpolatorNode<GMArray<DirectX::XMFLOAT2>, InterpolatorType::TARGETED>::MixKeys(const GMArray<DirectX::XMFLOAT2>& first, const GMArray<DirectX::XMFLOAT2>& second, const float coef);

template<>
VertexList XG_InterpolatorNode<VertexList, InterpolatorType::TIMED>::MixKeys(const VertexList& first, const VertexList& second, const float coef);
