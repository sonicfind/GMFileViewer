#include "xgShapeInterpolator.h"

void xgShapeInterpolator::updateVertexBuffer() const
{
	const VertexList interpolatedKeys = getInterpolatedKey();
	// Use applicable "SubData" command
}

void xgShapeInterpolator::loadKeys(FilePointer& file)
{
	if (m_keys.reserve(file))
		for (auto& element : m_keys)
			element.load(file);
}
