#include "xgShapeInterpolator.h"

void xgShapeInterpolator::updateVertices(VertexList& vertices) const
{
	vertices = getInterpolatedKey();
}

void xgShapeInterpolator::loadKeys(FilePointer& file)
{
	if (m_keys.reserve(file))
		for (auto& element : m_keys)
			element.load(file);
}
