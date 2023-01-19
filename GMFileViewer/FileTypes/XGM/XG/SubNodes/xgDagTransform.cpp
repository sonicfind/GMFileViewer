#include "xgDagTransform.h"
#include "../PString.h"

void xgDagTransform::load(FilePointer& file, const XG* xg)
{
	m_inputMatrix = static_cast<xgBgMatrix*>(xg->grabNode_optional("inputMatrix", "outputMatrix", file));
}

DirectX::XMMATRIX xgDagTransform::calcTransformMatrix() const
{
	if (!m_inputMatrix)
		return DirectX::XMMatrixIdentity();
	return m_inputMatrix->transform().getMatrix();
}
