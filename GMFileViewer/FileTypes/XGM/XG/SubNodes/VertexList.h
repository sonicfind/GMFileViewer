#pragma once
#include <glm/glm.hpp>
#include "GMArray.h"

struct Vertex
{
	glm::vec4 m_position;
	glm::vec3 m_normal;
	glm::vec4 m_color;
	glm::vec2 m_texCoord;

	template <uint32_t flags>
	void fill(FileReader& file)
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
	void write(FileWriter& file) const
	{
		if constexpr (flags == 15)
			file.write(this, sizeof(Vertex));
		else if constexpr (flags == 7)
			file.write(this, 11 * sizeof(float));
		else if constexpr (flags == 3)
			file.write(this, 7 * sizeof(float));
		else
		{
			if constexpr (flags & 1)
				file.write(m_position);

			if constexpr (flags & 2)
				file.write(m_normal);

			if constexpr (flags & 4)
				file.write(m_color);

			if constexpr (flags & 8)
				file.write(m_texCoord);
		}
	}

	template <uint32_t flags>
	static Vertex mix(Vertex vertex, const Vertex& toMix, float coef)
	{
		if constexpr (flags & 1)
			vertex.m_position = glm::mix(vertex.m_position, toMix.m_position, coef);

		if constexpr (flags & 2)
			vertex.m_normal = glm::mix(vertex.m_normal, toMix.m_normal, coef);

		if constexpr (flags & 4)
			vertex.m_color = glm::mix(vertex.m_color, toMix.m_color, coef);

		if constexpr (flags & 8)
			vertex.m_texCoord = glm::mix(vertex.m_texCoord, toMix.m_texCoord, coef);
		return vertex;
	}
};

glm::vec4 operator*(const glm::mat4& matrix, const Vertex& vertex);

class VertexList
{
	uint32_t m_vertexFlags = 0;
	GMArray<Vertex> m_vertices;

	std::vector<size_t> m_bufferIndices;

public:
	VertexList() = default;
	VertexList(const VertexList&) = default;
	VertexList(VertexList&&) = default;
	VertexList& operator=(VertexList&& list) noexcept;

	void load(FileReader& file);
	void save(FileWriter& file) const;
	VertexList mix(const VertexList& other, float coef) const;

	void createVertexBuffer(bool isDynamic);
	void addInstance(bool isDynamic);
	void bindBuffer(uint32_t instance) const;
	void updateBuffer() const;

	const Vertex& operator[](size_t index) const { return m_vertices[index]; }
	Vertex& operator[](size_t index) { return m_vertices[index]; }

private:
	template <uint32_t flags>
	static void FillVertices(GMArray<Vertex>& vertices, FileReader& file)
	{
		if constexpr (flags == 15)
		{
			vertices.reserve_and_fill(file);
			return;
		}

		vertices.reserve(file);
		for (auto& vertex : vertices)
			vertex.fill<flags>(file);
	}

	template <uint32_t flags>
	static void WriteVertices(const GMArray<Vertex>& vertices, FileWriter& file)
	{
		if constexpr (flags == 15)
		{
			vertices.write_full(file);
			return;
		}

		vertices.write_size(file);
		for (auto& vertex : vertices)
			vertex.write<flags>(file);
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
