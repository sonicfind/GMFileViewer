#include "xgEnvelope.h"

void xgEnvelope::load(FileReader& file, const XG* xg)
{
	PString::ReadNamedValue("startVertex", m_startVertex, file);
	PString::ReadNamedValue("weights", m_weights, file);
	PString::ReadNamedValue("vertexTargets", m_vertexTargets, file);

	for (size_t i = 0; i < MAX_BONES; ++i)
		if (!(m_inputMatrices[i] = static_cast<xgBone*>(xg->grabNode_optional(MATRIXNAMES[i], "envelopeMatrix", file))))
			break;

	m_inputGeometry = static_cast<xgBgGeometry*>(xg->grabNode("inputGeometry", "outputGeometry", file));
}

void xgEnvelope::writeType(FileWriter& file) const
{
	PString::WriteString("xgEnvelope", file);
}

void xgEnvelope::save(FileWriter& file) const
{
	PString::WriteNamedValue("startVertex", m_startVertex, file);
	PString::WriteNamedValue("weights", m_weights, file);
	PString::WriteNamedValue("vertexTargets", m_vertexTargets, file);

	for (size_t i = 0; i < MAX_BONES && m_inputMatrices[i]; ++i)
		WriteNode(MATRIXNAMES[i], "envelopeMatrix", m_inputMatrices[i], file);

	 WriteNode("inputGeometry", "outputGeometry", m_inputGeometry, file);
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
