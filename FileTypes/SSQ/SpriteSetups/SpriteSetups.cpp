#include "SpriteSetups.h"

void SpritesSetup::read(const char*& input)
{
	if (!FileOps::checkTag("GMSP", input))
		throw "Sprite Setup read error";

	const uint32_t headerVersion = FileOps::Read<uint32_t>(input);
	input += 28;

	const uint32_t numFixed = FileOps::Read<uint32_t>(input);
	const uint32_t numUnk_1 = FileOps::Read<uint32_t>(input);
	const uint32_t numUnk_2 = FileOps::Read<uint32_t>(input);
	input += 4;

	if (numFixed > 0)
		m_fixedSpriteSetup.read(input);

	if (numUnk_1 > 0 || numUnk_2 > 0)
		throw "Unsuppored sprite type. Please send this file to the GitarooPals discord for research.";
}
