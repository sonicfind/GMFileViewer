#pragma once
#include <DirectXMath.h>
#include "GMArray.h"

struct Vertex
{
	DirectX::XMFLOAT4 m_position;
	DirectX::XMFLOAT3 m_normal;
	DirectX::XMFLOAT4 m_color;
	DirectX::XMFLOAT2 m_texCoord;

	template <uint32_t flags>
	void fill(FilePointer& file)
	{
		if constexpr (flags == 15)
			file.read(this, sizeof(Vertex));
		else if constexpr (flags == 7)
			file.read(this, 11 * sizeof(float));
		else if constexpr (flags == 3)
			file.read(this, 7 * sizeof(float));
		else
		{
			if constexpr (flags & 1)
				file.read(m_position);

			if constexpr (flags & 2)
				file.read(m_normal);

			if constexpr (flags & 4)
				file.read(m_color);

			if constexpr (flags & 8)
				file.read(m_texCoord);
		}
	}

	template <uint32_t flags>
	static Vertex mix(const Vertex& lhs, const Vertex& rhs, float coef)
	{
		Vertex result;
		if constexpr (flags & 1)
			DirectX::XMStoreFloat4(&result.m_position, DirectX::XMVectorLerp(DirectX::XMLoadFloat4(&lhs.m_position), DirectX::XMLoadFloat4(&rhs.m_position), coef));

		if constexpr (flags & 2)
			DirectX::XMStoreFloat3(&result.m_normal, DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&lhs.m_normal), DirectX::XMLoadFloat3(&rhs.m_normal), coef));

		if constexpr (flags & 4)
			DirectX::XMStoreFloat4(&result.m_color, DirectX::XMVectorLerp(DirectX::XMLoadFloat4(&lhs.m_color), DirectX::XMLoadFloat4(&rhs.m_color), coef));

		if constexpr (flags & 8)
			DirectX::XMStoreFloat2(&result.m_texCoord, DirectX::XMVectorLerp(DirectX::XMLoadFloat2(&lhs.m_texCoord), DirectX::XMLoadFloat2(&rhs.m_texCoord), coef));
		return result;
	}
};

Vertex operator*(const DirectX::XMMATRIX& matrix, Vertex vertex);

class VertexList
{
	uint32_t m_vertexFlags = 0;
	GMArray<Vertex> m_vertices;

public:
	void load(FilePointer& file);
	VertexList mix(const VertexList& other, float coef) const;
	const Vertex& operator[](size_t index) const { return m_vertices[index]; }

private:
	template <uint32_t flags>
	static void fillVertices(GMArray<Vertex>& vertices, FilePointer& file)
	{
		vertices.reserve(file);
		for (auto& vertex : vertices)
			vertex.fill<flags>(file);
	}

	template <uint32_t flags>
	static GMArray<Vertex> mix(const GMArray<Vertex>& lhs, const GMArray<Vertex>& rhs, float coef)
	{
		if constexpr (flags == 0)
			return lhs;

		GMArray<Vertex> result(lhs.getSize());
		for (uint32_t i = 0; i < lhs.getSize(); ++i)
			result[i] = Vertex::mix<flags>(lhs[i], rhs[i], coef);
		return result;
	}
};