#include "LightSetup.h"

void LightSetup::read(FilePointer& file)
{
	file.read(m_baseValues);
	if (!m_baseValues.isActive)
		return;

	if (!file.checkTag("GMLT"))
		throw "Light Setup read error";

	file.read(m_headerVersion);
	file += 28;

	const uint32_t numRotations = file.read<uint32_t>();
	const uint32_t numColors = file.read<uint32_t>();
	if (numRotations >= 2)
		m_rotations.reserve_and_fill(file, numRotations);

	if (numColors >= 2)
		m_colors.reserve_and_fill(file, numColors);
}
