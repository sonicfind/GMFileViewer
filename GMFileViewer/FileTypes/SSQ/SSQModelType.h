#pragma once
#include <stdint.h>

enum class SSQModelType : uint32_t
{
	Normal,
	Player1,
	Player2,
	DuetPlayer,
	Player1AttDef = 5,
	Player2AttDef,
	DuetPlayerAttDef,
	DuetComboAttack,
	Snake
};

enum ModelDrawStatus : uint32_t
{
	NoDraw,
	Draw,
	Draw_withShadow
};
