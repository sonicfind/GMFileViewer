#pragma once
#include <glm/gtx/quaternion.hpp>
#include "GMArray.h"

enum class InterpolationToggle : uint32_t
{
	Off,
	On,
};

template <typename T>
struct Keyframe
{
	alignas(16) float time;
	float coefficient;
	T object;
	InterpolationToggle interpolation;

	T* operator->()
	{
		return &object;
	}

	const T* operator->() const
	{
		return &object;
	}

	friend bool operator<(float currFrame, const Keyframe& keyframe)
	{
		return currFrame < keyframe.time;
	}
};

template <typename T>
using KeyFrameArray = GMArray<Keyframe<T>>;

template <typename T>
const Keyframe<T>* Iterate(const KeyFrameArray<T>& keyframes, float currFrame)
{
	return std::upper_bound(keyframes.begin(), keyframes.end(), currFrame) - 1;
}

template <typename T>
T Interpolate(const KeyFrameArray<T>& keyframes, float currFrame)
{
	const Keyframe<T>* iter = Iterate(keyframes, currFrame);
	if (iter + 1 == keyframes.end() || iter->interpolation != InterpolationToggle::On)
		return iter->object;
	
	return glm::mix(iter->object, (iter + 1)->object, (currFrame - iter->time) * iter->coefficient);
}

template <>
glm::quat Interpolate<glm::quat>(const KeyFrameArray<glm::quat>& keyframes, float currFrame);

template <typename T, typename = std::enable_if<std::is_arithmetic<T>::value>>
T Mix(T first, T second, float coef)
{
	return T(first + (second - first) * coef);
}

template <typename T>
T InterpolateStruct(const KeyFrameArray<T>& keyframes, float currFrame)
{
	const Keyframe<T>* iter = Iterate(keyframes, currFrame);
	if (iter + 1 == keyframes.end() || iter->interpolation != InterpolationToggle::On)
		return iter->object;

	return T::Mix(iter->object, (iter + 1)->object, (currFrame - iter->time) * iter->coefficient);
}
