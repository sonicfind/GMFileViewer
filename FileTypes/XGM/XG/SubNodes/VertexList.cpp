#include "VertexList.h"
#include "FileOperations.h"

void VertexList::load(const char*& input)
{
	FileOps::Read(m_vertexFlags, input);
	FileOps::Read(m_vertices.size, input);

	m_vertices.elements = std::make_unique<Vertex[]>(m_vertices.size);
	switch (m_vertexFlags)
	{
	case 0:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<0>(input);
		break;
	case 1:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<1>(input);
		break;
	case 2:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<2>(input);
		break;
	case 3:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<3>(input);
		break;
	case 4:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<4>(input);
		break;
	case 5:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<5>(input);
		break;
	case 6:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<6>(input);
		break;
	case 7:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<7>(input);
		break;
	case 8:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<8>(input);
		break;
	case 9:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<9>(input);
		break;
	case 10:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<10>(input);
		break;
	case 11:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<11>(input);
		break;
	case 12:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<12>(input);
		break;
	case 13:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<13>(input);
		break;
	case 14:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<14>(input);
		break;
	case 15:
		for (uint32_t i = 0; i < m_vertices.size; ++i)
			m_vertices.elements[i].fill<15>(input);
		break;
	}
}
