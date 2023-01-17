#include "xgEnvelope.h"
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

void xgEnvelope::updateVertexBuffer() const
{
	using namespace DirectX;
	const DirectX::XMMATRIX boneMatrices[4] =
	{
		m_inputMatrices[0] ? m_inputMatrices[0]->calcTransformMatrix() : XMMatrixIdentity(),
		m_inputMatrices[1] ? m_inputMatrices[1]->calcTransformMatrix() : XMMatrixIdentity(),
		m_inputMatrices[2] ? m_inputMatrices[2]->calcTransformMatrix() : XMMatrixIdentity(),
		m_inputMatrices[3] ? m_inputMatrices[3]->calcTransformMatrix() : XMMatrixIdentity(),
	};

	for (size_t index = 0, targetIndex = 0; index < m_weights.getSize(); ++index, ++targetIndex)
	{
		const Vertex vertex = (m_weights[index].values[0] * boneMatrices[0] +
		                       m_weights[index].values[1] * boneMatrices[1] +
		                       m_weights[index].values[2] * boneMatrices[2] +
		                       m_weights[index].values[3] * boneMatrices[3]) * m_inputGeometry->getVertex(m_startVertex + index);

		while (m_vertexTargets[index] != -1)
		{
			// Update the vertex buffer at this target's index
			++index;
		}
	}
}
