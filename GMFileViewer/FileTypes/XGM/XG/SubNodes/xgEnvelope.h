#pragma once
#include "XG_UpdatableNodes.h"
#include "xgBone.h"
#include "xgBgGeometry.h"

class xgEnvelope : public XG_SubNode, public XG_UpdatableNode
{
	static constexpr int MAX_BONES = 4;
	static constexpr std::string_view MATRIXNAMES[MAX_BONES] = { "inputMatrix1", "inputMatrix2", "inputMatrix3", "inputMatrix4" };

	struct Weight
	{
		float values[MAX_BONES];
	};

	uint32_t m_startVertex = 0;
	GMArray<Weight> m_weights;
	GMArray<int32_t> m_vertexTargets;

	xgBone* m_inputMatrices[4]{};
	xgBgGeometry* m_inputGeometry = nullptr;

public:
	void load(FilePointer& file, const XG* xg) override;

	void writeType(FileWriter& file) const override;
	void save(FileWriter& file, const XG* xg) const override;

	void updateVertices(VertexList& vertices) const override;
};

