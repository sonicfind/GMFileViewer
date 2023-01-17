#include "CameraSetup.h"

void CameraSetup::read(FilePointer& file)
{
	if (!file.checkTag("GMPX"))
		throw "Camera Setup read error";

	const uint32_t headerVersion = file.read<uint32_t>();

	file += 28;
	file.read(m_baseGlobalValues);
	m_positions.reserve(file);
	m_rotations.reserve(file);
	m_positions.fill(file);
	m_rotations.fill(file);

	const uint32_t numProjections = file.read<uint32_t>();
	if (numProjections >= 2)
		m_projections.reserve_and_fill(file, numProjections);

	const uint32_t numColors = file.read<uint32_t>();
	if (numColors >= 2)
		m_ambientColors.reserve_and_fill(file, numColors);

	m_lights.reserve(file);
	for (auto& light : m_lights)
		light.read(file);

	if (headerVersion >= 0x1200)
	{
		const uint32_t numUnknown = file.read<uint32_t>();
		if (numUnknown >= 2)
			m_64bytes_v.reserve_and_fill(file, numUnknown);
	}
}
