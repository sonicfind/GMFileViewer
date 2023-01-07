#include "VertexList.h"
#include "FileOperations.h"

void VertexList::load(const char*& input)
{
	FileOps::Read(m_vertexFlags, input);
	if (m_vertexFlags >= 16)
		throw "why'd you edit the vertexFlags to an invalid value you dumb SOB?";

	if (!m_vertices.reserve(input))
		return;

	static constexpr void (*vertexFillers[])(GMArray<Vertex>&, const char*&) =
	{
		VertexList::fillVertices<0>,  VertexList::fillVertices<1>,  VertexList::fillVertices<2>,  VertexList::fillVertices<3>,
		VertexList::fillVertices<4>,  VertexList::fillVertices<5>,  VertexList::fillVertices<6>,  VertexList::fillVertices<7>,
		VertexList::fillVertices<8>,  VertexList::fillVertices<9>,  VertexList::fillVertices<10>, VertexList::fillVertices<11>,
		VertexList::fillVertices<12>, VertexList::fillVertices<13>, VertexList::fillVertices<14>, VertexList::fillVertices<15>,
	};

	vertexFillers[m_vertexFlags](m_vertices, input);
}
