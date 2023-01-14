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

	static T Interpolate(const GMArray<Keyframe<T>>& keyframes, float currFrame)
	{
		const Keyframe* iter = std::upper_bound(keyframes.begin(), keyframes.end(), currFrame);
		if (iter == keyframes.end() || (iter - 1)->interpolation != InterpolationToggle::On)
			return (iter - 1)->object;

		return T::mix(iter->object, (iter + 1)->object, (currFrame - iter->time) * iter->coefficient);
	}
};

struct XMQuat : public DirectX::XMFLOAT4 {};

template<>
DirectX::XMFLOAT3 Keyframe<DirectX::XMFLOAT3>::Interpolate(const GMArray<Keyframe<DirectX::XMFLOAT3>>& keyframes, float currFrame);

template<>
DirectX::XMFLOAT4 Keyframe<DirectX::XMFLOAT4>::Interpolate(const GMArray<Keyframe<DirectX::XMFLOAT4>>& keyframes, float currFrame);

template<>
XMQuat Keyframe<XMQuat>::Interpolate(const GMArray<Keyframe<XMQuat>>& keyframes, float currFrame);

using PositionKeyframes = GMArray<Keyframe<DirectX::XMFLOAT3>>;
using RotationKeyframes = GMArray<Keyframe<XMQuat>>;

template <typename T>
using KeyFrameArray = GMArray<Keyframe<T>>;
