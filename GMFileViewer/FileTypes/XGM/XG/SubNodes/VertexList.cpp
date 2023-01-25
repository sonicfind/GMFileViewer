#include "VertexList.h"
#include "Graphics.h"

VertexList& VertexList::operator=(VertexList&& list) noexcept
{
	m_vertices = std::move(list.m_vertices);
	return *this;
}

void VertexList::load(FileReader& file)
{
	file.read(m_vertexFlags);
	if (m_vertexFlags >= 16)
		throw "why'd you edit the vertexFlags to an invalid value you dumb SOB?";

	static constexpr void (*vertexFillers[])(GMArray<Vertex>&, FileReader&) =
	{
		VertexList::FillVertices<0>,  VertexList::FillVertices<1>,  VertexList::FillVertices<2>,  VertexList::FillVertices<3>,
		VertexList::FillVertices<4>,  VertexList::FillVertices<5>,  VertexList::FillVertices<6>,  VertexList::FillVertices<7>,
		VertexList::FillVertices<8>,  VertexList::FillVertices<9>,  VertexList::FillVertices<10>, VertexList::FillVertices<11>,
		VertexList::FillVertices<12>, VertexList::FillVertices<13>, VertexList::FillVertices<14>, VertexList::FillVertices<15>,
	};

	vertexFillers[m_vertexFlags](m_vertices, file);
}

void VertexList::save(FileWriter& file) const
{
	file.write(m_vertexFlags);

	static constexpr void (*vertexWriters[])(const GMArray<Vertex>&, FileWriter&) =
	{
		VertexList::WriteVertices<0>,  VertexList::WriteVertices<1>,  VertexList::WriteVertices<2>,  VertexList::WriteVertices<3>,
		VertexList::WriteVertices<4>,  VertexList::WriteVertices<5>,  VertexList::WriteVertices<6>,  VertexList::WriteVertices<7>,
		VertexList::WriteVertices<8>,  VertexList::WriteVertices<9>,  VertexList::WriteVertices<10>, VertexList::WriteVertices<11>,
		VertexList::WriteVertices<12>, VertexList::WriteVertices<13>, VertexList::WriteVertices<14>, VertexList::WriteVertices<15>,
	};

	vertexWriters[m_vertexFlags](m_vertices, file);
}

VertexList VertexList::mix(const VertexList& other, float coef) const
{
	static constexpr GMArray<Vertex> (*vertexMixers[])(const GMArray<Vertex>&, const GMArray<Vertex>&, float) =
	{
		VertexList::mix<0>,  VertexList::mix<1>,  VertexList::mix<2>,  VertexList::mix<3>,
		VertexList::mix<4>,  VertexList::mix<5>,  VertexList::mix<6>,  VertexList::mix<7>,
		VertexList::mix<8>,  VertexList::mix<9>,  VertexList::mix<10>, VertexList::mix<11>,
		VertexList::mix<12>, VertexList::mix<13>, VertexList::mix<14>, VertexList::mix<15>,
	};

	VertexList result;
	result.m_vertexFlags = m_vertexFlags;
	result.m_vertices = vertexMixers[m_vertexFlags](m_vertices, other.m_vertices, coef);
	return result;
}

void VertexList::createVertexBuffer(bool isDynamic)
{
	m_bufferIndex = Graphics::getGraphics()->createVertexBuffer(Graphics::ShaderType::Model, m_vertices.begin(), m_vertices.getSize() * sizeof(Vertex), isDynamic);
}

void VertexList::bindBuffer() const
{
	Graphics::getGraphics()->bindVertexBuffer(m_bufferIndex);
}

void VertexList::updateBuffer() const
{
	Graphics::getGraphics()->updateVertexBuffer(0, m_vertices.begin(), m_vertices.getSize() * sizeof(Vertex));
}

DirectX::XMVECTOR operator*(const DirectX::XMMATRIX& matrix, const Vertex& vertex)
{
	return XMVector4Transform(DirectX::XMLoadFloat4(&vertex.m_position), matrix);
}
