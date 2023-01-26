#include "xgTexCoordInterpolator.h"

void xgTexCoordInterpolator::writeType(FileWriter& file) const
{
	PString::WriteString("xgTexCoordInterpolator", file);
}

void xgTexCoordInterpolator::updateVertices(VertexList& vertices) const
{
	const auto interpolatedKeys = getInterpolatedKey();
	for (uint32_t targetIndex = 0; targetIndex < m_targets.getSize(); ++targetIndex)
		vertices[m_targets[targetIndex]].m_texCoord = interpolatedKeys[targetIndex];
}
