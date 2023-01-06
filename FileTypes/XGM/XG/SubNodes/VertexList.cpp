#include "VertexList.h"
#include "FileOperations.h"

void VertexList::load(const char*& input)
{
	FileOps::Read(m_vertexFlags, input);
	if (!m_vertices.reserve(input))
		return;

	switch (m_vertexFlags)
	{
	case 0:
		for (auto& vertex : m_vertices)
			vertex.fill<0>(input);
		break;
	case 1:
		for (auto& vertex : m_vertices)
			vertex.fill<1>(input);
		break;
	case 2:
		for (auto& vertex : m_vertices)
			vertex.fill<2>(input);
		break;
	case 3:
		for (auto& vertex : m_vertices)
			vertex.fill<3>(input);
		break;
	case 4:
		for (auto& vertex : m_vertices)
			vertex.fill<4>(input);
		break;
	case 5:
		for (auto& vertex : m_vertices)
			vertex.fill<5>(input);
		break;
	case 6:
		for (auto& vertex : m_vertices)
			vertex.fill<6>(input);
		break;
	case 7:
		for (auto& vertex : m_vertices)
			vertex.fill<7>(input);
		break;
	case 8:
		for (auto& vertex : m_vertices)
			vertex.fill<8>(input);
		break;
	case 9:
		for (auto& vertex : m_vertices)
			vertex.fill<9>(input);
		break;
	case 10:
		for (auto& vertex : m_vertices)
			vertex.fill<10>(input);
		break;
	case 11:
		for (auto& vertex : m_vertices)
			vertex.fill<11>(input);
		break;
	case 12:
		for (auto& vertex : m_vertices)
			vertex.fill<12>(input);
		break;
	case 13:
		for (auto& vertex : m_vertices)
			vertex.fill<13>(input);
		break;
	case 14:
		for (auto& vertex : m_vertices)
			vertex.fill<14>(input);
		break;
	case 15:
		for (auto& vertex : m_vertices)
			vertex.fill<15>(input);
		break;
	}
}
