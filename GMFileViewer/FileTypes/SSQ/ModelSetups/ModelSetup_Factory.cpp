#include "Model_Setup.h"
#include "PlayerModel_Setup.h"
#include "AttDefModel_Setup.h"
#include "SnakeModel_Setup.h"

std::unique_ptr<Model_Setup> Model_Setup::create(FilePointer& file, SSQModelType type)
{
	switch (type)
	{
	case SSQModelType::Normal:
		return std::unique_ptr<Model_Setup>(new Model_Setup(file));
	case SSQModelType::Player1:
	case SSQModelType::Player2:
	case SSQModelType::DuetPlayer:
		return std::unique_ptr<Model_Setup>(new PlayerModel_Setup(file));
	case SSQModelType::Player1AttDef:
	case SSQModelType::Player2AttDef:
	case SSQModelType::DuetPlayerAttDef:
	case SSQModelType::DuetComboAttack:
		return std::unique_ptr<Model_Setup>(new AttDefModel_Setup(file));
	case SSQModelType::Snake:
		return std::unique_ptr<Model_Setup>(new SnakeModel_Setup(file));
	default:
		throw "Bad modelSetup type";
	}
}