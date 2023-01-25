#include "xgBgMatrix.h"
#include "../PString.h"
#include "FileReader.h"

void xgBgMatrix::load(FileReader& file, const XG* xg)
{
	PString::ReadNamedValue("position", m_position, file);
	PString::ReadNamedValue("rotation", m_rotation, file);
	PString::ReadNamedValue("scale", m_scale, file);

	while (xg->grabNode_nondestructive(m_inputPosition     , "inputPosition", "outputVec3", file) ||
		   xg->grabNode_nondestructive(m_inputRotation     , "inputRotation", "outputQuat", file) ||
		   xg->grabNode_nondestructive(m_inputScale        , "inputScale", "outputVec3", file) ||
		   xg->grabNode_nondestructive(m_inputParentMatrix , "inputParentMatrix", "outputMatrix", file));
}

void xgBgMatrix::writeType(FileWriter& file) const
{
	PString::WriteString("xgBgMatrix", file);
}

void xgBgMatrix::save(FileWriter& file) const
{
	PString::WriteNamedValue("position", m_position, file);
	PString::WriteNamedValue("rotation", m_rotation, file);
	PString::WriteNamedValue("scale", m_scale, file);

	if (m_inputPosition)
		WriteNode("inputPosition", "outputVec3", m_inputPosition, file);

	if (m_inputRotation)
		WriteNode("inputRotation", "outputQuat", m_inputRotation, file);

	if (m_inputScale)
		WriteNode("inputScale", "outputVec3", m_inputScale, file);

	if (m_inputParentMatrix)
		WriteNode("inputParentMatrix", "outputMatrix", m_inputParentMatrix, file);
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
