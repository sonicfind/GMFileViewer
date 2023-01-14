#include "xgEnvelope.h"
#include "xgBgGeometry.h"
#include "../PString.h"
#include "FileOperations.h"

void xgEnvelope::load(const char*& input, const XG* xg)
{
	PString::ReadNamedValue("startVertex", m_startVertex, input);
	PString::ReadNamedValue("weights", m_weights, input);
	PString::ReadNamedValue("vertexTargets", m_vertexTargets, input);

	static constexpr std::string_view MATRICES[MAX_BONES] = { "inputMatrix1", "inputMatrix2", "inputMatrix3", "inputMatrix4"};

	for (size_t i = 0; i < MAX_BONES; ++i)
		if (!(m_inputMatrices[i] = static_cast<xgBone*>(xg->grabNode_optional(MATRICES[i], "envelopeMatrix", input))))
			break;

	m_inputGeometry = static_cast<xgBgGeometry*>(xg->grabNode_optional("inputGeometry", "outputGeometry", input));
}
