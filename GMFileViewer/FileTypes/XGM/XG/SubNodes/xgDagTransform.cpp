#include "xgDagTransform.h"
#include "../PString.h"

void xgDagTransform::load(FilePointer& file, const XG* xg)
{
	m_inputMatrix = static_cast<xgBgMatrix*>(xg->grabNode_optional("inputMatrix", "outputMatrix", file));
}

void xgDagTransform::writeType(FileWriter& file) const
{
	PString::WriteString("xgDagTransform", file);
}

void xgDagTransform::save(FileWriter& file, const XG* xg) const
{
	if (m_inputMatrix)
		xg->writeNode("inputMatrix", "outputMatrix", m_inputMatrix, file);
}

DirectX::XMMATRIX xgDagTransform::calcTransformMatrix() const
{
	if (!m_inputMatrix)
		return DirectX::XMMatrixIdentity();
	return m_inputMatrix->transform();
}
