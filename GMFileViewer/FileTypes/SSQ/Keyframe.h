#pragma once
#include <DirectXMath.h>
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

	friend bool operator<(float currFrame, const Keyframe& keyframe)
	{
		return currFrame < keyframe.time;
	}
};

template <typename T>
using KeyFrameArray = GMArray<Keyframe<T>>;

template <typename T, typename = std::enable_if<!std::is_same_v<T, DirectX::XMFLOAT3> && !std::is_same_v<T, DirectX::XMFLOAT4>>>
T InterpolateStruct(const KeyFrameArray<T>& keyframes, float currFrame)
{
	const Keyframe<T>* iter = std::upper_bound(keyframes.begin(), keyframes.end(), currFrame) - 1;
	if (iter + 1 == keyframes.end() || iter->interpolation != InterpolationToggle::On)
		return iter->object;

	return T::mix(iter->object, (iter + 1)->object, (currFrame - iter->time) * iter->coefficient);
}

DirectX::XMVECTOR InterpolateFloat3(const KeyFrameArray<DirectX::XMFLOAT3>& keyframes, float currFrame);
DirectX::XMVECTOR InterpolateFloat4(const KeyFrameArray<DirectX::XMFLOAT4>& keyframes, float currFrame);
DirectX::XMVECTOR InterpolateRotation(const KeyFrameArray<DirectX::XMFLOAT4>& keyframes, float currFrame);


