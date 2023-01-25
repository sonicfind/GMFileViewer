#pragma once
#include "Model_Setup.h"

class AttDefModel_Setup : public Model_Setup
{
	friend class Model_Setup;

	float m_attackSize_Z;

	struct AttackStreamValues
	{
		DirectX::XMFLOAT3A startOffset;
		DirectX::XMFLOAT3 targetOffset;
		char startingModel[16];
		char targetModel[16];
	} m_attackValues;

private:
	AttDefModel_Setup(FilePointer& file);

public:
	void save(FileWriter& file) const override;
};
