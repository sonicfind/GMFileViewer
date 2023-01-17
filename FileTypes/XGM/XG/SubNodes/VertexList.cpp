#include "VertexList.h"
#include "FileOperations.h"

void VertexList::load(const char*& input)
{
	FileOps::Read(m_vertexFlags, input);
	if (m_vertexFlags >= 16)
		throw "why'd you edit the vertexFlags to an invalid value you dumb SOB?";

	static constexpr void (*vertexFillers[])(GMArray<Vertex>&, const char*&) =
	{
		VertexList::fillVertices<0>,  VertexList::fillVertices<1>,  VertexList::fillVertices<2>,  VertexList::fillVertices<3>,
		VertexList::fillVertices<4>,  VertexList::fillVertices<5>,  VertexList::fillVertices<6>,  VertexList::fillVertices<7>,
		VertexList::fillVertices<8>,  VertexList::fillVertices<9>,  VertexList::fillVertices<10>, VertexList::fillVertices<11>,
		VertexList::fillVertices<12>, VertexList::fillVertices<13>, VertexList::fillVertices<14>, VertexList::fillVertices<15>,
	};

	vertexFillers[m_vertexFlags](m_vertices, input);
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

Vertex operator*(const DirectX::XMMATRIX& matrix, const Vertex& vertex)
{
	using namespace DirectX;

	Vertex result(vertex);
	DirectX::XMStoreFloat4(&result.m_position, XMVector4Transform(DirectX::XMLoadFloat4(&result.m_position), matrix));
	DirectX::XMStoreFloat3(&result.m_normal, XMVector3TransformNormal(DirectX::XMLoadFloat3(&result.m_normal), matrix));
	return result;
}
