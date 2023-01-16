#include "xgBone.h"
#include "../PString.h"
#include "FileOperations.h"

void xgBone::load(const char*& input, const XG* xg)
{
	PString::ReadNamedValue("restMatrix", m_restMatrix, input);
	m_inputMatrix = static_cast<xgBgMatrix*>(xg->grabNode("inputMatrix", "outputMatrix", input));
}

DirectX::XMMATRIX xgBone::calcTransformMatrix() const
{
	if (!m_inputMatrix)
		return m_restMatrix;

	auto transforms = m_inputMatrix->transform();
	return DirectX::XMMatrixRotationQuaternion(transforms.rotation) *
		DirectX::XMMatrixScalingFromVector(transforms.scale) *
		DirectX::XMMatrixTranslationFromVector(transforms.translation) *
		m_restMatrix;
}
