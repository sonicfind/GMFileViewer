#include "xgShapeInterpolator.h"
#include "Graphics.h"

void xgShapeInterpolator::updateVertexBuffer() const
{
	getInterpolatedKey().updateBuffer();
}

void xgShapeInterpolator::loadKeys(FilePointer& file)
{
	if (m_keys.reserve(file))
		for (auto& element : m_keys)
			element.load(file);
}
