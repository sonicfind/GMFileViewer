#include "TextureAnimation.h"

void TexAnim::read(const char*& input)
{
	if (!FileOps::checkTag("\0\0\0\0", input))
		throw "Texture Animation read error";

	const uint32_t headerVersion = FileOps::Read<uint32_t>(input);
	input += 28;

	FileOps::Read(m_offset, input);
	FileOps::Read(m_textureName, input);
	input += 8;

	m_cutOuts.reserve_and_fill(input);
	m_cutoutTimeline.reserve_and_fill(input);
}
