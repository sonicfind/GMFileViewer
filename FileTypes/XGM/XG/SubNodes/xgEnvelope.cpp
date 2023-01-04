#include "xgEnvelope.h"
#include "xgBgGeometry.h"
#include "../PString.h"
#include "FileOperations.h"

void xgEnvelope::load(const char*& input, const std::vector<XGVectElement>& nodes)
{
	PString::ReadNamedValue("startVertex", m_startVertex, input);
	PString::ReadNamedValue("weights", m_weights, input);
	PString::ReadNamedValue("vertexTargets", m_vertexTargets, input);

	static constexpr std::string_view MATRICES[MAX_BONES] = { "inputMatrix1", "inputMatrix2", "inputMatrix3", "inputMatrix4"};
	{
		size_t i = 0;
		while (i < MAX_BONES && BindNode_optional(m_inputMatrices[i], MATRICES[i], "envelopeMatrix", input, nodes))
			++i;
	}

	BindNode_optional(m_inputGeometry, "inputGeometry", "outputGeometry", input, nodes);
}
