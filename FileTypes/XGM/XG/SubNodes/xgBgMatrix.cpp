#include "xgBgMatrix.h"
#include "../PString.h"
#include "FileOperations.h"

void xgBgMatrix::load(const char*& input, const std::vector<XGVectElement>& nodes)
{
	PString::ReadNamedValue("position", m_position, input);
	PString::ReadNamedValue("rotation", m_rotation, input);
	PString::ReadNamedValue("scale", m_scale, input);

	while (BindNode_optional(m_inputPosition, "inputPosition", "outputVec3", input, nodes) ||
		   BindNode_optional(m_inputRotation, "inputRotation", "outputQuat", input, nodes) ||
		   BindNode_optional(m_inputScale, "inputScale", "outputVec3", input, nodes) ||
		   BindNode_optional(m_inputParentMatrix, "inputParentMatrix", "outputMatrix", input, nodes));
}
