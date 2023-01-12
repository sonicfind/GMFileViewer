#include "Keyframe.h"

template<>
glm::vec3 Keyframe<glm::vec3>::Interpolate(const GMArray<Keyframe<glm::vec3>>& keyframes, float currFrame)
{
	const auto* iter = std::upper_bound(keyframes.begin(), keyframes.end(), currFrame);
	if (iter == keyframes.end())
		return (iter - 1)->object;

	return glm::mix(iter->object, (iter + 1)->object, (currFrame - iter->time) * iter->coefficient);
}

template<>
glm::vec4 Keyframe<glm::vec4>::Interpolate(const GMArray<Keyframe<glm::vec4>>& keyframes, float currFrame)
{
	const auto* iter = std::upper_bound(keyframes.begin(), keyframes.end(), currFrame);
	if (iter == keyframes.end())
		return (iter - 1)->object;

	return glm::mix(iter->object, (iter + 1)->object, (currFrame - iter->time) * iter->coefficient);
}

template<>
glm::quat Keyframe<glm::quat>::Interpolate(const GMArray<Keyframe<glm::quat>>& keyframes, float currFrame)
{
	const auto* iter = std::upper_bound(keyframes.begin(), keyframes.end(), currFrame);
	if (iter == keyframes.end())
		return (iter - 1)->object;

	return glm::slerp(iter->object, (iter + 1)->object, (currFrame - iter->time) * iter->coefficient);
}
