#pragma once
#include "Model_Setup.h"

class AttDefModel_Setup : public Model_Setup
{
	friend class Model_Setup;

	float m_attackSize_Z;

	struct AttackStreamValues
	{
		glm::vec3 startOffset;
		glm::vec3 targetOffset;
		char startingModel[16];
		char targetModel[16];
	} m_attackValues;

private:
	AttDefModel_Setup(FileReader& file);

public:
	void save(FileWriter& file) const override;
};
