#pragma once
#include "Triangle_Data.h"
#include "xgBgGeometry.h"
#include "xgMultiPassMaterial.h"

class xgDagMesh : public XG_SubNode
{
	uint32_t m_primType = 0;
	Triangle_Data<PrimitiveType::PRIMITIVE> m_prim;
	Triangle_Data<PrimitiveType::TRIANGLE_FAN> m_triFan;
	Triangle_Data<PrimitiveType::TRIANGLE_STRIP> m_triStrip;
	Triangle_Data<PrimitiveType::TRIANGLE_LIST> m_triList;
	uint32_t m_cullFunc = 0;

	xgBgGeometry* m_inputGeometry = nullptr;
	XG_MaterialNode* m_inputMaterial = nullptr;

public:
	void load(const char*& input, const XG* xg) override;
};

