#include "TextureAnimation.h"

void TexAnim::read(FileReader& file)
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

void TexAnim::save(FileWriter& file) const
{
	file.writeTag("\0\0\0\0");
	file << uint32_t(0x1000);

	static constexpr char ZERO[12]{};
	static char garbo[16];
	file << ZERO << garbo;

	file.write(m_offset);
	file.write(m_textureName);
	file.write(ZERO, 8);

	m_cutOuts.write_full(file);
	m_cutoutTimeline.write_full(file);
}
