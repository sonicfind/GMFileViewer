#include "Keyframe.h"

template <>
glm::quat Interpolate<glm::quat>(const KeyFrameArray<glm::quat>& keyframes, float currFrame)
{
	const Keyframe<glm::quat>* iter = Iterate(keyframes, currFrame);
	if (iter + 1 == keyframes.end() || iter->interpolation != InterpolationToggle::On)
		return iter->object;
	
	return glm::slerp(iter->object, (iter + 1)->object, (currFrame - iter->time) * iter->coefficient);
}
