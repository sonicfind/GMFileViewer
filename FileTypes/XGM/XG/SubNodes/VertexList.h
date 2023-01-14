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
	void fill(const char*& input)
	{
		if constexpr (flags == 15)
			FileOps::Read(this, input, sizeof(Vertex));
		else if constexpr (flags == 7)
			FileOps::Read(this, input, 11 * sizeof(float));
		else if constexpr (flags == 3)
			FileOps::Read(this, input, 7 * sizeof(float));
		else
		{
			if constexpr (flags & 1)
				FileOps::Read(m_position, input);

			if constexpr (flags & 2)
				FileOps::Read(m_normal, input);

			if constexpr (flags & 4)
				FileOps::Read(m_color, input);

			if constexpr (flags & 8)
				FileOps::Read(m_texCoord, input);
		}
	}
};

class VertexList
{
	uint32_t m_vertexFlags = 0;
	GMArray<Vertex> m_vertices;

	template <uint32_t flags>
	static void fillVertices(GMArray<Vertex>& vertices, const char*& input)
	{
		if constexpr (flags == 0)
			return;

		for (auto& vertex : vertices)
			vertex.fill<flags>(input);
	}

public:
	void load(const char*& input);
};
