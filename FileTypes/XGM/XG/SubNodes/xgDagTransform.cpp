#include "xgDagTransform.h"
#include "../PString.h"

void xgDagTransform::load(const char*& input, const XG* xg)
{
	m_inputMatrix = static_cast<xgBgMatrix*>(xg->grabNode_optional("inputMatrix", "outputMatrix", input));
}
