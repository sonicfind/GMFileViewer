#include "LightSetup.h"

void LightSetup::read(FileReader& file)
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

void LightSetup::save(FileWriter& file) const
{
	file.write(m_baseValues);
	if (!m_baseValues.isActive)
		return;

	file.writeTag("GMLT");
	file.write(m_headerVersion);

	static constexpr char ZERO[12]{};
	static char garbo[16];
	file << ZERO << garbo;

	m_rotations.write_size(file);
	m_colors.write_size(file);
	if (m_rotations.getSize() >= 2)
		m_rotations.write_data(file);

	if (m_colors.getSize() >= 2)
		m_colors.write_data(file);
}
