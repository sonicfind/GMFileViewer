#include "FixedSprite_Setup.h"

void FixedSpriteSetup::read(FilePointer& file)
{
	if (!file.checkTag("GMF0"))
		throw "Sprite Setup read error";

	const uint32_t headerVersion = file.read<uint32_t>();
	file += 28;

	const uint32_t numSprites = file.read<uint32_t>();
	m_80bytes.reserve_and_fill(file, numSprites);
	m_fixedSprites.reserve(numSprites);
	
	for (auto& sprite : m_fixedSprites)
		sprite.read(file);
}
