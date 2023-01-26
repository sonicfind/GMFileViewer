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
	using XG_SubNode::XG_SubNode;
	void load(FileReader& file, const XG* xg) override
	{
		PString::ReadNamedValue("type", m_type, file);

		if constexpr (INTERPOLATION >= InterpolatorType::TIMED)
			PString::ReadNamedValue("times", m_times, file);

		PString::ThrowOnStringMismatch("keys", file);
		loadKeys(file);

		if constexpr (INTERPOLATION == InterpolatorType::TARGETED)
			PString::ReadNamedValue("targets", m_targets, file);

		m_inputTime = static_cast<xgTime*>(xg->grabNode("inputTime", "outputTime", file));
	}

	void save(FileWriter& file) const override
	{
		PString::WriteNamedValue("type", m_type, file);

		if constexpr (INTERPOLATION >= InterpolatorType::TIMED)
			PString::WriteNamedValue("times", m_times, file);

		PString::WriteString("keys", file);
		saveKeys(file);

		if constexpr (INTERPOLATION == InterpolatorType::TARGETED)
			PString::WriteNamedValue("targets", m_targets, file);

		WriteNode("inputTime", "outputTime", m_inputTime, file);
	}

protected:
	T getInterpolatedKey() const
	{
		return interpolateFrame(calculateTimeFrame());
	}

	virtual void loadKeys(FileReader& file)
	{
		m_keys.reserve_and_fill(file);
	}

	virtual void saveKeys(FileWriter& file) const
	{
		m_keys.write_full(file);
	}

	virtual T mixKeys(const T& first, const T& second, float coef) const = 0;

private:
	Frame calculateTimeFrame() const
	{
		float time = m_inputTime->getTime();
		float frame = floorf(time);
		if constexpr (INTERPOLATION < InterpolatorType::TIMED)
		{
			if (frame + 1 >= m_keys.getSize())
				return { frame, frame, 0 };
			return { frame, frame + 1, time - frame };
		}
		else if (uint32_t(time) >= m_times.getSize() - 1)
		{
			time = m_times.back();
			frame = floorf(time);
			return { frame, frame + 1, time - frame };
		}
		else
		{
			const size_t index = size_t(time);
			return { m_times[index], m_times[index + 1], time - floorf(time) };
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
			return mixKeys(m_keys[index], m_keys[index + 1], time - index);
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
			return mixKeys(getKeyPoint(frame.first), getKeyPoint(frame.second), frame.coef);
	}
};
