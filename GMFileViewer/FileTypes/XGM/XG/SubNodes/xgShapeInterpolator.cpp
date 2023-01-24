#include "xgShapeInterpolator.h"

void xgShapeInterpolator::writeType(FileWriter& file) const
{
	PString::WriteString("xgShapeInterpolator", file);
}

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

void xgShapeInterpolator::saveKeys(FileWriter& file) const
{
	m_keys.write_size(file);
	for (const auto& key : m_keys)
		key.save(file);
}
