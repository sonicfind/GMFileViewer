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
