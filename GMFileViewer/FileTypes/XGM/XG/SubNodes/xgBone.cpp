#include "xgBone.h"
#include "../PString.h"
#include "FilePointer.h"

void xgBone::load(FilePointer& file, const XG* xg)
{
	PString::ReadNamedValue("restMatrix", m_restMatrix, file);
	m_inputMatrix = static_cast<xgBgMatrix*>(xg->grabNode("inputMatrix", "outputMatrix", file));
}

void xgBone::writeType(FileWriter& file) const
{
	PString::WriteString("xgBone", file);
}

void xgBone::save(FileWriter& file, const XG* xg) const
{
	PString::WriteNamedValue("restMatrix", m_restMatrix, file);
	xg->writeNode("inputMatrix", "outputMatrix", m_inputMatrix, file);
}

DirectX::XMMATRIX xgBone::calcTransformMatrix() const
{
	if (!m_inputMatrix)
		return m_restMatrix;

	return m_restMatrix *  m_inputMatrix->transform();
}
