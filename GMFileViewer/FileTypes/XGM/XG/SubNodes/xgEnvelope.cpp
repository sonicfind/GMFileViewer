#include "xgEnvelope.h"

void xgEnvelope::load(FilePointer& file, const XG* xg)
{
	PString::ReadNamedValue("startVertex", m_startVertex, file);
	PString::ReadNamedValue("weights", m_weights, file);
	PString::ReadNamedValue("vertexTargets", m_vertexTargets, file);

	static constexpr std::string_view MATRICES[MAX_BONES] = { "inputMatrix1", "inputMatrix2", "inputMatrix3", "inputMatrix4"};

	for (size_t i = 0; i < MAX_BONES; ++i)
		if (!(m_inputMatrices[i] = static_cast<xgBone*>(xg->grabNode_optional(MATRICES[i], "envelopeMatrix", file))))
			break;

	m_inputGeometry = static_cast<xgBgGeometry*>(xg->grabNode_optional("inputGeometry", "outputGeometry", file));
}

void xgEnvelope::updateVertices(VertexList& vertices) const
{
	using namespace DirectX;

	static const auto IDENTITY = XMMatrixIdentity();
	const DirectX::XMMATRIX boneMatrices[4] =
	{
		m_inputMatrices[0] ? m_inputMatrices[0]->calcTransformMatrix() : IDENTITY,
		m_inputMatrices[1] ? m_inputMatrices[1]->calcTransformMatrix() : IDENTITY,
		m_inputMatrices[2] ? m_inputMatrices[2]->calcTransformMatrix() : IDENTITY,
		m_inputMatrices[3] ? m_inputMatrices[3]->calcTransformMatrix() : IDENTITY,
	};

	for (size_t index = 0, targetIndex = 0; index < m_weights.getSize(); ++index, ++targetIndex)
	{
		const XMVECTOR position = boneMatrices[0] * m_inputGeometry->getVertex(m_startVertex + index);
		while (m_vertexTargets[targetIndex] != -1)
		{
			XMStoreFloat4(&vertices[m_vertexTargets[targetIndex]].m_position, position);
			++targetIndex;
		}
	}
}
