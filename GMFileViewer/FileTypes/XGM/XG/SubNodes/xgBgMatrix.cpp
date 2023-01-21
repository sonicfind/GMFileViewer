#include "xgBgMatrix.h"
#include "../PString.h"
#include "FilePointer.h"

void xgBgMatrix::load(FilePointer& file, const XG* xg)
{
	PString::ReadNamedValue("position", m_position, file);
	PString::ReadNamedValue("rotation", m_rotation, file);
	PString::ReadNamedValue("scale", m_scale, file);

	while (xg->grabNode_nondestructive(m_inputPosition     , "inputPosition", "outputVec3", file) ||
		   xg->grabNode_nondestructive(m_inputRotation     , "inputRotation", "outputQuat", file) ||
		   xg->grabNode_nondestructive(m_inputScale        , "inputScale", "outputVec3", file) ||
		   xg->grabNode_nondestructive(m_inputParentMatrix , "inputParentMatrix", "outputMatrix", file));
}

using namespace DirectX;

XMMATRIX xgBgMatrix::transform() const
{
	XMMATRIX matrix = m_inputParentMatrix ? m_inputParentMatrix->transform() : XMMatrixIdentity();
	if (!m_inputScale)
		matrix *= XMMatrixScalingFromVector(XMLoadFloat3(&m_scale));
	else
	{
		const XMFLOAT3 scale = m_inputScale->calcMixedValue();
		matrix *= XMMatrixScalingFromVector(XMLoadFloat3(&scale));
	}

	matrix *= XMMatrixRotationQuaternion(XMQuaternionConjugate(m_inputRotation ? m_inputRotation->calcMixedValue() : m_rotation));

	if (!m_inputPosition)
		matrix *= XMMatrixTranslationFromVector(XMLoadFloat3(&m_position));
	else
	{
		const XMFLOAT3 translation = m_inputPosition->calcMixedValue();
		matrix *= XMMatrixTranslationFromVector(XMLoadFloat3(&translation));
	}
	return matrix;
}
