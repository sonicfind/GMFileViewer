#include "Keyframe.h"

template<>
DirectX::XMFLOAT3 Keyframe<DirectX::XMFLOAT3>::Interpolate(const GMArray<Keyframe<DirectX::XMFLOAT3>>& keyframes, float currFrame)
{
	const auto* iter = std::upper_bound(keyframes.begin(), keyframes.end(), currFrame);
	if (iter == keyframes.end())
		return (iter - 1)->object;

	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&iter->object), DirectX::XMLoadFloat3(&(iter + 1)->object), (currFrame - iter->time) * iter->coefficient));
	return result;
}

template<>
DirectX::XMFLOAT4 Keyframe<DirectX::XMFLOAT4>::Interpolate(const GMArray<Keyframe<DirectX::XMFLOAT4>>& keyframes, float currFrame)
{
	const auto* iter = std::upper_bound(keyframes.begin(), keyframes.end(), currFrame);
	if (iter == keyframes.end())
		return (iter - 1)->object;

	DirectX::XMFLOAT4 result;
	DirectX::XMStoreFloat4(&result, DirectX::XMVectorLerp(DirectX::XMLoadFloat4(&iter->object), DirectX::XMLoadFloat4(&(iter + 1)->object), (currFrame - iter->time) * iter->coefficient));
	return result;
}

template<>
XMQuat Keyframe<XMQuat>::Interpolate(const GMArray<Keyframe<XMQuat>>& keyframes, float currFrame)
{
	const auto* iter = std::upper_bound(keyframes.begin(), keyframes.end(), currFrame);
	if (iter == keyframes.end())
		return (iter - 1)->object;

	XMQuat result;
	DirectX::XMStoreFloat4(&result, DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(&iter->object), DirectX::XMLoadFloat4(&(iter + 1)->object), (currFrame - iter->time) * iter->coefficient));
	return result;
}
