#include "FixedSprite.h"
void FixedSprite::read(FilePointer& file)
{

	if (!file.checkTag("GMSP"))
		throw "Sprite Setup read error";

	const uint32_t headerVersion = file.read<uint32_t>();
	file += 28;

	file.read(m_64bytes);

	unsigned long numWorldValues, numColors, numFrames;
	file.read(numWorldValues);
	file.read(numColors);
	file.read(numFrames);

	if (numWorldValues >= 2)
		m_worldValues.reserve_and_fill(file, numWorldValues);

	if (numColors >= 2)
		m_colors.reserve_and_fill(file, numColors);

	if (numFrames > 2)
		m_spriteFrames.reserve_and_fill(file, numFrames);
}

void FixedSprite::save(FileWriter& file) const
{
	file.writeTag("GMSP");
	file << uint32_t(0x1000);

	static constexpr char ZERO[12]{};
	static char garbo[16];
	file << ZERO << garbo;

	file.write(m_64bytes);

	m_worldValues.write_size(file);
	m_colors.write_size(file);
	m_spriteFrames.write_size(file);

	if (m_worldValues.getSize() >= 2)
		m_worldValues.write_data(file);

	if (m_colors.getSize() >= 2)
		m_colors.write_data(file);

	if (m_spriteFrames.getSize() > 2)
		m_spriteFrames.write_data(file);
}
