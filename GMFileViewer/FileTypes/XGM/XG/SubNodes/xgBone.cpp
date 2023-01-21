#include "xgBone.h"
#include "../PString.h"
#include "FilePointer.h"

void xgBone::load(FilePointer& file, const XG* xg)
{
	PString::ReadNamedValue("restMatrix", m_restMatrix, file);
	m_inputMatrix = static_cast<xgBgMatrix*>(xg->grabNode("inputMatrix", "outputMatrix", file));
}

DirectX::XMMATRIX xgBone::calcTransformMatrix() const
{
	if (!m_inputMatrix)
		return m_restMatrix;

	return m_restMatrix *  m_inputMatrix->transform();
}
