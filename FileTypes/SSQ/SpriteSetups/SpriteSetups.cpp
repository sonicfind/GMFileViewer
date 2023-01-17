#include "SpriteSetups.h"

void SpritesSetup::read(FilePointer& file)
{
	if (!file.checkTag("GMSP"))
		throw "Sprite Setup read error";

	const uint32_t headerVersion = file.read<uint32_t>();
	file += 28;

	const uint32_t numFixed = file.read<uint32_t>();
	const uint32_t numUnk_1 = file.read<uint32_t>();
	const uint32_t numUnk_2 = file.read<uint32_t>();
	file += 4;

	if (numFixed > 0)
		m_fixedSpriteSetup.read(file);

	if (numUnk_1 > 0 || numUnk_2 > 0)
		throw "Unsuppored sprite type. Please send this file to the GitarooPals discord for research.";
}
