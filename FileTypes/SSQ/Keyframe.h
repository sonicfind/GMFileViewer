#pragma once
#include "GMArray.h"
#include <glm/glm/gtx/quaternion.hpp>

enum class InterpolationToggle : uint32_t
{
	On,
	Off
};

template <typename T>
struct Keyframe
{
	alignas(16) float time;
	float coefficient;
	T object;
	InterpolationToggle interpolation;

	friend bool operator<(float currFrame, const Keyframe& keyframe)
	{
		return currFrame < keyframe.time;
	}

	static T Interpolate(const GMArray<Keyframe<T>>& keyframes, float currFrame)
	{
		const Keyframe* iter = std::upper_bound(keyframes.begin(), keyframes.end(), currFrame);
		if (iter == keyframes.end() || (iter - 1)->interpolation != InterpolationToggle::On)
			return (iter - 1)->object;

		return T::mix(iter->object, (iter + 1)->object, (currFrame - iter->time) * iter->coefficient);
	}
};

template<>
glm::vec3 Keyframe<glm::vec3>::Interpolate(const GMArray<Keyframe<glm::vec3>>& keyframes, float currFrame);

template<>
glm::vec4 Keyframe<glm::vec4>::Interpolate(const GMArray<Keyframe<glm::vec4>>& keyframes, float currFrame);

template<>
glm::quat Keyframe<glm::quat>::Interpolate(const GMArray<Keyframe<glm::quat>>& keyframes, float currFrame);

using PositionKeyframes = GMArray<Keyframe<glm::vec3>>;
using RotationKeyframes = GMArray<Keyframe<glm::quat>>;

template <typename T>
using KeyFrameArray = GMArray<Keyframe<T>>;
