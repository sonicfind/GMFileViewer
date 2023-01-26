#include "xgBone.h"
#include "../PString.h"
#include "FileReader.h"

void xgBone::load(FileReader& file, const XG* xg)
{
	PString::ReadNamedValue("restMatrix", m_restMatrix, file);
	m_inputMatrix = static_cast<xgBgMatrix*>(xg->grabNode("inputMatrix", "outputMatrix", file));
}

void xgBone::writeType(FileWriter& file) const
{
	PString::WriteString("xgBone", file);
}

void xgBone::save(FileWriter& file) const
{
	PString::WriteNamedValue("restMatrix", m_restMatrix, file);
	WriteNode("inputMatrix", "outputMatrix", m_inputMatrix, file);
}

glm::mat4 xgBone::calcTransformMatrix() const
{
	if (!m_inputMatrix)
		return m_restMatrix;

	return m_inputMatrix->transform() * m_restMatrix;
}
