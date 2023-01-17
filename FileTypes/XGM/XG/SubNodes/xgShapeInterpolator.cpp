#include "xgShapeInterpolator.h"

void xgShapeInterpolator::updateVertexBuffer() const
{
	const VertexList interpolatedKeys = getInterpolatedKey();
	// Use applicable "SubData" command
}

void xgShapeInterpolator::loadKeys(const char*& input)
{
	if (m_keys.reserve(input))
		for (auto& element : m_keys)
			element.load(input);
}
