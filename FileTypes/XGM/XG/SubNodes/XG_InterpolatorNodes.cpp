#include "XG_InterpolatorNodes.h"

template<>
void XG_InterpolatorNode<GMArray<DirectX::XMFLOAT3>, InterpolatorType::TARGETED>::loadKeys(const char*& input)
{
	if (m_keys.reserve(input))
		for (auto& element : m_keys)
			element.reserve_and_fill(input);
}

template<>
void XG_InterpolatorNode<GMArray<DirectX::XMFLOAT2>, InterpolatorType::TARGETED>::loadKeys(const char*& input)
{
	if (m_keys.reserve(input))
		for (auto& element : m_keys)
			element.reserve_and_fill(input);
}


template<>
void XG_InterpolatorNode<VertexList, InterpolatorType::TIMED>::loadKeys(const char*& input)
{
	if (m_keys.reserve(input))
		for (auto& element : m_keys)
			element.load(input);
}


template<>
DirectX::XMFLOAT3 XG_InterpolatorNode<DirectX::XMFLOAT3>::Interpolate(const DirectX::XMFLOAT3& first, const DirectX::XMFLOAT3& second, const float coef)
{
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&first), DirectX::XMLoadFloat3(&second), coef));
	return result;
}

template<>
DirectX::XMVECTOR XG_InterpolatorNode<DirectX::XMVECTOR>::Interpolate(const DirectX::XMVECTOR& first, const DirectX::XMVECTOR& second, const float coef)
{
	return DirectX::XMVectorLerp(first, second, coef);
}


template<>
GMArray<DirectX::XMFLOAT3> XG_InterpolatorNode<GMArray<DirectX::XMFLOAT3>, InterpolatorType::TARGETED>::Interpolate(const GMArray<DirectX::XMFLOAT3>& first, const GMArray<DirectX::XMFLOAT3>& second, const float coef)
{
	GMArray<DirectX::XMFLOAT3> result(first.getSize());
	for (uint32_t i = 0; i < result.getSize(); ++i)
		DirectX::XMStoreFloat3(&result[i], DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&first[i]), DirectX::XMLoadFloat3(&second[i]), coef));
	return result;
}


template<>
GMArray<DirectX::XMFLOAT2> XG_InterpolatorNode<GMArray<DirectX::XMFLOAT2>, InterpolatorType::TARGETED>::Interpolate(const GMArray<DirectX::XMFLOAT2>& first, const GMArray<DirectX::XMFLOAT2>& second, const float coef)
{
	GMArray<DirectX::XMFLOAT2> result(first.getSize());
	for (uint32_t i = 0; i < result.getSize(); ++i)
		DirectX::XMStoreFloat2(&result[i], DirectX::XMVectorLerp(DirectX::XMLoadFloat2(&first[i]), DirectX::XMLoadFloat2(&second[i]), coef));
	return result;
}


template<>
VertexList XG_InterpolatorNode<VertexList, InterpolatorType::TIMED>::Interpolate(const VertexList& first, const VertexList& second, const float coef)
{
	return first.mix(second, coef);
}
