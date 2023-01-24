#pragma once
#include "xgBgMatrix.h"

class xgBone : public XG_SubNode
{
	DirectX::XMMATRIX m_restMatrix;
	xgBgMatrix* m_inputMatrix = nullptr;

public:
	void load(FilePointer& file, const XG* xg) override;

	void writeType(FileWriter& file) const override;
	void save(FileWriter& file, const XG* xg) const override;

	DirectX::XMMATRIX calcTransformMatrix() const;
};

