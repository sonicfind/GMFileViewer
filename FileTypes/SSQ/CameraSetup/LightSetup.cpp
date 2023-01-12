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

	m_rotations.reserve(input);
	m_colors.reserve(input);
	m_rotations.fill(input);
	m_colors.fill(input);
}
