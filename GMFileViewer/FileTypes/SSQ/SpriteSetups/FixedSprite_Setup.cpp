#include "FixedSprite_Setup.h"

void FixedSpriteSetup::read(FileReader& file)
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

void FixedSpriteSetup::save(FileWriter& file) const
{
	file.writeTag("GMF0");
	file << uint32_t(0x1000);
	
	static constexpr char ZERO[12]{};
	static char garbo[16];
	file << ZERO << garbo;

	m_80bytes.write_size(file);
	m_80bytes.write_data(file);

	for (const auto& sprite : m_fixedSprites)
		sprite.save(file);
}

uint32_t FixedSpriteSetup::getNumSprites() const
{
	return m_fixedSprites.getSize();
}
