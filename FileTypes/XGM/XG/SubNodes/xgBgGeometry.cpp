#include "xgBgGeometry.h"
#include "../PString.h"
#include "FileOperations.h"

void xgBgGeometry::load(const char*& input, const std::vector<XGVectElement>& nodes)
{
	PString::ReadNamedValue("density", m_density, input);
	PString::ReadNamedValue("vertices", m_vertices, input);
	BindNodes(m_inputGeometries, "inputGeometry", "outputGeometry", input, nodes);
}
