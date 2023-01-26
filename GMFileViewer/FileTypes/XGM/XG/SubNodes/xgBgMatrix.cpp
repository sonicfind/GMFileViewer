#include "xgBgMatrix.h"
#include "../PString.h"
#include "FileReader.h"
#include <glm/gtc/matrix_transform.hpp>

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

using namespace glm;

glm::mat4 xgBgMatrix::transform() const
{
	Transformations transforms = gatherTransformations();

	mat4 result = glm::toMat4(conjugate(transforms.rotation));
	result[0] *= transforms.scale.x;
	result[1] *= transforms.scale.y;
	result[2] *= transforms.scale.z;
	result[3] = vec4(transforms.translation, 1);
	return result;
}

xgBgMatrix::Transformations xgBgMatrix::gatherTransformations() const
{
	Transformations transforms;
	if (m_inputParentMatrix)
		transforms = m_inputParentMatrix->gatherTransformations();

	transforms.translation += m_inputPosition ? m_inputPosition->calcMixedValue() : m_position;
	transforms.rotation *= m_inputRotation ? m_inputRotation->calcMixedValue() : m_rotation;
	transforms.scale *= m_inputScale ? m_inputScale->calcMixedValue() : m_scale;
	return transforms;
}


