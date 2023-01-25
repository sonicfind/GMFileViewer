#pragma once
#include "FixedSprite_Setup.h"

class SpritesSetup
{
	FixedSpriteSetup m_fixedSpriteSetup;

public:
	void read(FileReader& file);
	void save(FileWriter& file) const;
};
