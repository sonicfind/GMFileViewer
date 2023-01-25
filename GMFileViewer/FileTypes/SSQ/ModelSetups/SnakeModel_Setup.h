#pragma once
#include "Model_Setup.h"

class SnakeModel_Setup : public Model_Setup
{
	friend class Model_Setup;
private:
	SnakeModel_Setup(FileReader& file);
};
