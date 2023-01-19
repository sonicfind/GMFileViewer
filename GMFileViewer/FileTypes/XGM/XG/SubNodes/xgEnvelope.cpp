#include "xgEnvelope.h"
#include "Graphics.h"

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

void xgEnvelope::updateVertexBuffer() const
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

	const Graphics* gfx = Graphics::getGraphics();
	for (size_t index = 0, targetIndex = 0; index < m_weights.getSize(); ++index, ++targetIndex)
	{
		DirectX::XMMATRIX mat{};
		{
			float total = 0;
			for (size_t i = 0; i < 4 && total < 1; ++i)
			{
				mat += m_weights[index].values[i] * boneMatrices[i];
				total += m_weights[index].values[i];
			}
		}

		const Vertex vertex = mat * m_inputGeometry->getVertex(m_startVertex + index);

		while (m_vertexTargets[targetIndex] != -1)
		{
			gfx->updateVertexBuffer(m_vertexTargets[targetIndex] * sizeof(Vertex), &vertex, sizeof(DirectX::XMFLOAT3) + sizeof(DirectX::XMFLOAT4));
			++targetIndex;
		}
	}
}
