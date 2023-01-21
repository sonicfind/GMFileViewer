#include "xgTexCoordInterpolator.h"

void xgTexCoordInterpolator::updateVertices(VertexList& vertices) const
{
	const auto interpolatedKeys = getInterpolatedKey();
	for (uint32_t targetIndex = 0; targetIndex < m_targets.getSize(); ++targetIndex)
		memcpy(&vertices[m_targets[targetIndex]].m_texCoord, &interpolatedKeys[targetIndex], sizeof(DirectX::XMFLOAT2));
}
