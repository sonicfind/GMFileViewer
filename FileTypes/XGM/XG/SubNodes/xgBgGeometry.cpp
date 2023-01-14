#include "xgBgGeometry.h"
#include "../PString.h"
#include "FileOperations.h"

void xgBgGeometry::load(const char*& input, const XG* xg)
{
	PString::ReadNamedValue("density", m_density, input);
	PString::ReadNamedValue("vertices", m_vertices, input);

	while (XG_SubNode* node = xg->grabNode_optional("inputGeometry", "outputGeometry", input))
		m_inputGeometries.push_back(node);
}
