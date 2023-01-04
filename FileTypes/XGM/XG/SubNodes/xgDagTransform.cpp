#include "xgDagTransform.h"
#include "../PString.h"

void xgDagTransform::load(const char*& input, const std::vector<XGVectElement>& nodes)
{
	BindNode_optional(m_inputMatrix, "inputMatrix", "outputMatrix", input, nodes);
}
