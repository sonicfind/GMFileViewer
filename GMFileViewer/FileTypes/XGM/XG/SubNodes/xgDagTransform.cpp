#include "xgDagTransform.h"
#include "../PString.h"

void xgDagTransform::load(FileReader& file, const XG* xg)
{
	m_inputMatrix = static_cast<xgBgMatrix*>(xg->grabNode_optional("inputMatrix", "outputMatrix", file));
}

void xgDagTransform::writeType(FileWriter& file) const
{
	PString::WriteString("xgDagTransform", file);
}

void xgDagTransform::save(FileWriter& file) const
{
	if (m_inputMatrix)
		WriteNode("inputMatrix", "outputMatrix", m_inputMatrix, file);
}

glm::mat4 xgDagTransform::calcTransformMatrix() const
{
	if (!m_inputMatrix)
		return glm::identity<glm::mat4>();
	return m_inputMatrix->transform();
}
