#include "LightSetup.h"

void LightSetup::read(const char*& input)
{
	FileOps::Read(m_baseValues, input);
	if (!m_baseValues.isActive)
		return;

	if (!FileOps::checkTag("GMLT", input))
		throw "Light Setup read error";

	FileOps::Read(m_headerVersion, input);
	input += 28;

	const uint32_t numRotations = FileOps::Read<uint32_t>(input);
	const uint32_t numColors = FileOps::Read<uint32_t>(input);
	if (numRotations >= 2)
		m_rotations.reserve_and_fill(input, numRotations);

	if (numColors >= 2)
		m_colors.reserve_and_fill(input, numColors);
}
