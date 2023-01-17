#pragma once
#include "xgBone.h"

class xgBgGeometry;
class xgEnvelope : public XG_SubNode
{
	static constexpr int MAX_BONES = 4;
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
	void load(const char*& input, const XG* xg) override;
	void update() const;
};

