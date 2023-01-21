#include "VertexList.h"
#include "Graphics.h"

VertexList& VertexList::operator=(VertexList&& list)
{
	m_vertices = std::move(list.m_vertices);
	return *this;
}

void VertexList::load(FilePointer& file)
{
	file.read(m_vertexFlags);
	if (m_vertexFlags >= 16)
		throw "why'd you edit the vertexFlags to an invalid value you dumb SOB?";

	static constexpr void (*vertexFillers[])(GMArray<Vertex>&, FilePointer&) =
	{
		VertexList::fillVertices<0>,  VertexList::fillVertices<1>,  VertexList::fillVertices<2>,  VertexList::fillVertices<3>,
		VertexList::fillVertices<4>,  VertexList::fillVertices<5>,  VertexList::fillVertices<6>,  VertexList::fillVertices<7>,
		VertexList::fillVertices<8>,  VertexList::fillVertices<9>,  VertexList::fillVertices<10>, VertexList::fillVertices<11>,
		VertexList::fillVertices<12>, VertexList::fillVertices<13>, VertexList::fillVertices<14>, VertexList::fillVertices<15>,
	};

	vertexFillers[m_vertexFlags](m_vertices, file);
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
	m_bufferIndex = Graphics::getGraphics()->createVertexBuffer(m_vertices.begin(), m_vertices.getSize() * sizeof(Vertex), isDynamic);
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
