#pragma once
#include "Triangle_Data.h"
#include "xgBgGeometry.h"
#include "xgMultiPassMaterial.h"

class xgDagMesh : public XG_SubNode
{
	uint32_t m_primType = 0;
	Triangle_Data<PrimitiveMode::PRIMITIVE> m_prim;
	Triangle_Data<PrimitiveMode::TRIANGLE_FAN> m_triFan;
	Triangle_Data<PrimitiveMode::TRIANGLE_STRIP> m_triStrip;
	Triangle_Data<PrimitiveMode::TRIANGLE_LIST> m_triList;
	Culling m_cullFunc = Culling::DISABLED;

	xgBgGeometry* m_inputGeometry = nullptr;
	XG_MaterialNode* m_inputMaterial = nullptr;

public:
	using XG_SubNode::XG_SubNode;
	void load(FileReader& file, const XG* xg) override;

	void writeType(FileWriter& file) const override;
	void save(FileWriter& file) const override;

	void createVertexBuffer();
	void update();
	void draw(const glm::mat4& meshMatrix) const;
};

