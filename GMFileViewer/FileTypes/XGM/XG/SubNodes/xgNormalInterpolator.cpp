#include "xgNormalInterpolator.h"

void xgNormalInterpolator::writeType(FileWriter& file) const
{
	PString::WriteString("xgNormalInterpolator", file);
}

void xgNormalInterpolator::updateVertices(VertexList& vertices) const
{
	const auto interpolatedKeys = getInterpolatedKey();
	for (uint32_t targetIndex = 0; targetIndex < m_targets.getSize(); ++targetIndex)
		vertices[m_targets[targetIndex]].m_normal = interpolatedKeys[targetIndex];
}
