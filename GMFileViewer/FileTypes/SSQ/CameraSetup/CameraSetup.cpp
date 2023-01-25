#include "CameraSetup.h"

void CameraSetup::read(FileReader& file)
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

void CameraSetup::save(FileWriter& file) const
{
	file.writeTag("GMPX");
	file << 0x1100 + 0x100 * (m_64bytes_v.getSize() >= 2);

	static constexpr char ZERO[12]{};
	static char garbo[16];
	file << ZERO << garbo;

	file.write(m_baseGlobalValues);
	m_positions.write_size(file);
	m_rotations.write_size(file);
	m_positions.write_data(file);
	m_rotations.write_data(file);

	m_projections.write_size(file);
	if (m_projections.getSize() >= 2)
		m_projections.write_data(file);

	m_ambientColors.write_size(file);
	if (m_ambientColors.getSize() >= 2)
		m_ambientColors.write_data(file);

	m_lights.write_size(file);
	for (const auto& light : m_lights)
		light.save(file);

	if (m_64bytes_v.getSize() >= 2)
		m_64bytes_v.write_full(file);
}
