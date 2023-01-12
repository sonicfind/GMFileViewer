#include "CameraSetup.h"

void CameraSetup::read(const char*& input)
{
	if (!FileOps::checkTag("GMPX", input))
		throw "Camera Setup read error";

	const uint32_t headerVersion = FileOps::Read<uint32_t>(input);

	input += 28;
	FileOps::Read(m_baseGlobalValues, input);
	m_positions.reserve(input);
	m_rotations.reserve(input);
	m_positions.fill(input);
	m_rotations.fill(input);

	const uint32_t numProjections = FileOps::Read<uint32_t>(input);
	if (numProjections >= 2)
		m_projections.reserve_and_fill(input, numProjections);

	const uint32_t numColors = FileOps::Read<uint32_t>(input);
	if (numColors >= 2)
		m_ambientColors.reserve_and_fill(input, numColors);

	m_lights.reserve(input);
	for (auto& light : m_lights)
		light.read(input);

	if (headerVersion >= 0x1200)
	{
		const uint32_t numUnknown = FileOps::Read<uint32_t>(input);
		if (numUnknown >= 2)
			m_64bytes_v.reserve_and_fill(input, numUnknown);
	}
}
