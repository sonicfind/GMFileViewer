#include "XG_InterpolatorNodes.h"

template<>
DirectX::XMFLOAT3 XG_InterpolatorNode<DirectX::XMFLOAT3>::MixKeys(const DirectX::XMFLOAT3& first, const DirectX::XMFLOAT3& second, const float coef)
{
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&first), DirectX::XMLoadFloat3(&second), coef));
	return result;
}

template<>
DirectX::XMVECTOR XG_InterpolatorNode<DirectX::XMVECTOR>::MixKeys(const DirectX::XMVECTOR& first, const DirectX::XMVECTOR& second, const float coef)
{
	return DirectX::XMVectorLerp(first, second, coef);
}

template<>
GMArray<DirectX::XMFLOAT3> XG_InterpolatorNode<GMArray<DirectX::XMFLOAT3>, InterpolatorType::TARGETED>::MixKeys(const GMArray<DirectX::XMFLOAT3>& first, const GMArray<DirectX::XMFLOAT3>& second, const float coef)
{
	GMArray<DirectX::XMFLOAT3> result(first.getSize());
	for (uint32_t i = 0; i < result.getSize(); ++i)
		DirectX::XMStoreFloat3(&result[i], DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&first[i]), DirectX::XMLoadFloat3(&second[i]), coef));
	return result;
}


template<>
GMArray<DirectX::XMFLOAT2> XG_InterpolatorNode<GMArray<DirectX::XMFLOAT2>, InterpolatorType::TARGETED>::MixKeys(const GMArray<DirectX::XMFLOAT2>& first, const GMArray<DirectX::XMFLOAT2>& second, const float coef)
{
	GMArray<DirectX::XMFLOAT2> result(first.getSize());
	for (uint32_t i = 0; i < result.getSize(); ++i)
		DirectX::XMStoreFloat2(&result[i], DirectX::XMVectorLerp(DirectX::XMLoadFloat2(&first[i]), DirectX::XMLoadFloat2(&second[i]), coef));
	return result;
}


template<>
VertexList XG_InterpolatorNode<VertexList, InterpolatorType::TIMED>::MixKeys(const VertexList& first, const VertexList& second, const float coef)
{
	return first.mix(second, coef);
}
