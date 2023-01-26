#pragma once
#include "FixedSprite.h"
#include "../SSQModelType.h"
class FixedSpriteSetup
{
	struct Struct80_7f
	{
		unsigned long IMXindex;
		glm::vec2 initial_BottomLeft;
		glm::vec2 boxSize;
		glm::vec3 worldPosition;
		glm::vec2 worldSize;
		glm::vec2 worldSize_ZW_maybe;
		float float_l;
		unsigned long ulong_b;
		float float_m;
		unsigned long ulong_c;
		unsigned long depthTest;
		SSQModelType modelTypeMapping = SSQModelType::Normal;
		unsigned long ulong_f;
		unsigned long ulong_g;
	};

	GMArray<Struct80_7f> m_80bytes;
	GMArray<FixedSprite> m_fixedSprites;

public:
	void read(FileReader& file);
	void save(FileWriter& file) const;

	uint32_t getNumSprites() const;
};
