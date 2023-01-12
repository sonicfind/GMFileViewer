#pragma once
#include "Model_Setup.h"

class PlayerModel_Setup : public Model_Setup
{
	friend class Model_Setup;
	struct Controllable
	{
		alignas(16) float angleMin;
		float angleMax;
		uint32_t descriptor;
		uint32_t eventFlag;
		uint32_t animIndex;
		uint32_t holdTime;
		// 1 - forwards; 0 - backwards
		uint32_t playbackDirection;
		uint32_t interruptible;
		uint32_t useCurrentFrame_maybe;
		uint32_t randomize;
	};

	GMArray<Controllable> m_controllables;
	GMArray<GMArray<uint32_t>> m_connections;
	GMArray<int32_t> m_defaults;

private:
	PlayerModel_Setup(const char*& input);
};
