#include "xgBone.h"
#include "../PString.h"
#include "FileOperations.h"

void xgBone::load(const char*& input, const std::vector<XGVectElement>& nodes)
{
	PString::ReadNamedValue("restMatrix", m_restMatrix, input);
	BindNode(m_inputMatrix, "inputMatrix", "outputMatrix", input, nodes);
}
