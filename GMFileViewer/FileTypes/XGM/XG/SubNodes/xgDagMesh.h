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
	uint32_t m_cullFunc = 0;

	xgBgGeometry* m_inputGeometry = nullptr;
	XG_MaterialNode* m_inputMaterial = nullptr;

public:
	void load(FilePointer& file, const XG* xg) override;
	void update() const;
	void draw(const DirectX::XMMATRIX& meshMatrix) const;

private:
	void resetMaterialSlot() const;
	void bindGeometry() const;
		void setCulling() const;
};

