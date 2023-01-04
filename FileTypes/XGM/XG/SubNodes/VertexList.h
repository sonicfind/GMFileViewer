#pragma once
#include <Eigen/Dense>
#include "GMArray.h"

struct Vertex
{
	Eigen::Vector4f m_position;
	Eigen::Vector3f m_normal;
	Eigen::Vector4f m_color;
	Eigen::Vector2f m_texCoord;

	template <uint32_t flags>
	void fill(const char*& input)
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
};

class VertexList
{
	uint32_t m_vertexFlags = 0;
	GMArray<Vertex> m_vertices;

public:
	void load(const char*& input);
};
