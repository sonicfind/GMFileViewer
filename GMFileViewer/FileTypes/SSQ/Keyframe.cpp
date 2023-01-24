#include "Keyframe.h"

DirectX::XMVECTOR InterpolateFloat3(const KeyFrameArray<DirectX::XMFLOAT3>& keyframes, float currFrame)
{
	const auto* iter = std::upper_bound(keyframes.begin(), keyframes.end(), currFrame) - 1;
	if (iter + 1 == keyframes.end() || iter->interpolation != InterpolationToggle::On)
		return DirectX::XMLoadFloat3(&iter->object);

	return DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&iter->object), DirectX::XMLoadFloat3(&(iter + 1)->object), (currFrame - iter->time) * iter->coefficient);
}

DirectX::XMVECTOR InterpolateFloat4(const KeyFrameArray<DirectX::XMFLOAT4>& keyframes, float currFrame)
{
	const auto* iter = std::upper_bound(keyframes.begin(), keyframes.end(), currFrame) - 1;
	if (iter + 1 == keyframes.end() || iter->interpolation != InterpolationToggle::On)
		return DirectX::XMLoadFloat4(&iter->object);

	return DirectX::XMVectorLerp(DirectX::XMLoadFloat4(&iter->object), DirectX::XMLoadFloat4(&(iter + 1)->object), (currFrame - iter->time) * iter->coefficient);
}

DirectX::XMVECTOR InterpolateRotation(const KeyFrameArray<DirectX::XMFLOAT4>& keyframes, float currFrame)
{
	const auto* iter = std::upper_bound(keyframes.begin(), keyframes.end(), currFrame) - 1;
	if (iter + 1 == keyframes.end() || iter->interpolation != InterpolationToggle::On)
		return DirectX::XMLoadFloat4(&iter->object);

	return DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(&iter->object), DirectX::XMLoadFloat4(&(iter + 1)->object), (currFrame - iter->time) * iter->coefficient);
}
