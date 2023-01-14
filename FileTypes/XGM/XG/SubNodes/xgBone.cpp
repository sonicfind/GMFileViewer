#include "xgBone.h"
#include "../PString.h"
#include "FileOperations.h"

void xgBone::load(const char*& input, const XG* xg)
{
	PString::ReadNamedValue("restMatrix", m_restMatrix, input);
	m_inputMatrix = static_cast<xgBgMatrix*>(xg->grabNode("inputMatrix", "outputMatrix", input));
}
