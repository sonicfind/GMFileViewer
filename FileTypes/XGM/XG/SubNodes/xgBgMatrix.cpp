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
