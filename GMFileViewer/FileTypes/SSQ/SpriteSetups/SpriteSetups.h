#pragma once
#include "FixedSprite_Setup.h"

class SpritesSetup
{
	FixedSpriteSetup m_fixedSpriteSetup;

public:
	void read(FilePointer& file);
	void save(FileWriter& file) const;
};
