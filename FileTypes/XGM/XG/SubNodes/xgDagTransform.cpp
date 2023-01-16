#include "xgDagTransform.h"
#include "../PString.h"

void xgDagTransform::load(const char*& input, const XG* xg)
{
	m_inputMatrix = static_cast<xgBgMatrix*>(xg->grabNode_optional("inputMatrix", "outputMatrix", input));
}

DirectX::XMMATRIX xgDagTransform::calcTransformMatrix() const
{
	if (!m_inputMatrix)
		return DirectX::XMMatrixIdentity();

	auto transforms = m_inputMatrix->transform();
	return DirectX::XMMatrixRotationQuaternion(transforms.rotation) *
		DirectX::XMMatrixScalingFromVector(transforms.scale) *
		DirectX::XMMatrixTranslationFromVector(transforms.translation);
}
