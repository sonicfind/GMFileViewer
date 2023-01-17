#include "xgBgMatrix.h"
#include "../PString.h"
#include "FileOperations.h"

void xgBgMatrix::load(const char*& input, const XG* xg)
{
	PString::ReadNamedValue("position", m_position, input);
	PString::ReadNamedValue("rotation", m_rotation, input);
	PString::ReadNamedValue("scale", m_scale, input);

	while (xg->grabNode_nondestructive(m_inputPosition     , "inputPosition", "outputVec3", input) ||
		   xg->grabNode_nondestructive(m_inputRotation     , "inputRotation", "outputQuat", input) ||
		   xg->grabNode_nondestructive(m_inputScale        , "inputScale", "outputVec3", input) ||
		   xg->grabNode_nondestructive(m_inputParentMatrix , "inputParentMatrix", "outputMatrix", input));
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
