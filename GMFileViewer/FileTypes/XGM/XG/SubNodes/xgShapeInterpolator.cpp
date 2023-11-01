#include "xgShapeInterpolator.h"

void xgShapeInterpolator::writeType(FileWriter& file) const
{
	PString::WriteString("xgShapeInterpolator", file);
}

void xgShapeInterpolator::updateVertices(VertexList& vertices) const
{
	vertices = getInterpolatedKey();
}

void xgShapeInterpolator::loadKeys(FileReader& file)
{
	if (m_keys.construct(file))
		for (auto& element : m_keys)
			element.load(file);
}

void xgShapeInterpolator::saveKeys(FileWriter& file) const
{
	m_keys.write_size(file);
	for (const auto& key : m_keys)
		key.save(file);
}

VertexList xgShapeInterpolator::mixKeys(const VertexList& first, const VertexList& second, float coef) const
{
	return first.mix(second, coef);
}
