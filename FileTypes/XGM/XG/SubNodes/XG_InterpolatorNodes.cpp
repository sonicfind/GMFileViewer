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
