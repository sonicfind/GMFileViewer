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

xgBgMatrix::TransformVectors xgBgMatrix::transform() const
{
	using namespace DirectX;

	TransformVectors transformations;
	if (m_inputParentMatrix)
		transformations = m_inputParentMatrix->transform();

	if (m_inputPosition)
	{
		auto pos = m_inputPosition->calcMixedValue();
		transformations.translation += DirectX::XMLoadFloat3(&pos);
	}
	else
		transformations.translation += DirectX::XMLoadFloat3(&m_position);

	transformations.rotation = XMQuaternionMultiply(transformations.rotation, m_inputRotation ? m_inputRotation->calcMixedValue() : m_rotation);

	if (m_inputScale)
	{
		auto scl = m_inputScale->calcMixedValue();
		transformations.scale *= DirectX::XMLoadFloat3(&scl);
	}
	else
		transformations.scale *= DirectX::XMLoadFloat3(&m_scale);
	return transformations;
}
