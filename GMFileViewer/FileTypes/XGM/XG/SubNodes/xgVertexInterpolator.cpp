#include "xgVertexInterpolator.h"

void xgVertexInterpolator::writeType(FileWriter& file) const
{
	PString::WriteString("xgVertexInterpolator", file);
}

void xgVertexInterpolator::updateVertices(VertexList& vertices) const
{
	const auto interpolatedKeys = getInterpolatedKey();
	for (uint32_t targetIndex = 0; targetIndex < m_targets.getSize(); ++targetIndex)
		vertices[m_targets[targetIndex]].m_position = glm::vec4(interpolatedKeys[targetIndex], 1);
}
