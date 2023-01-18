#include "TextureAnimation.h"

void TexAnim::read(FilePointer& file)
{
	if (!file.checkTag("\0\0\0\0"))
		throw "Texture Animation read error";

	const uint32_t headerVersion = file.read<uint32_t>();
	file += 28;

	file.read(m_offset);
	file.read(m_textureName);
	file += 8;

	m_cutOuts.reserve_and_fill(file);
	m_cutoutTimeline.reserve_and_fill(file);
}
