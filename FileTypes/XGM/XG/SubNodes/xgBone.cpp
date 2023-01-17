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

	auto transforms = m_inputMatrix->transform();
	return DirectX::XMMatrixRotationQuaternion(transforms.rotation) *
		DirectX::XMMatrixScalingFromVector(transforms.scale) *
		DirectX::XMMatrixTranslationFromVector(transforms.translation) *
		m_restMatrix;
}
