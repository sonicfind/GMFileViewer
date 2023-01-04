#include "xgBgGeometry.h"
#include "../PString.h"
#include "FileOperations.h"

void xgBgGeometry::load(const char*& input, const std::vector<XGVectElement>& nodes)
{
	PString::ReadNamedValue("density", m_density, input);

	PString::CheckForString("vertices", input);
	m_vertices.load(input);

	BindNodes(m_inputGeometries, "inputGeometry", "outputGeometry", input, nodes);
}
