#include "Model_Setup.h"
#include "FileReader.h"

Model_Setup::Model_Setup(FileReader& file)
{
	if (!file.checkTag("GMPX"))
		throw "Model Setup read error";

	file.read(m_headerVersion);
	file += 28;

	m_positions.reserve(file);
	m_rotations.reserve(file);
	m_positions.fill(file);
	m_rotations.fill(file);

	const uint32_t numAnims = file.read<uint32_t>();
	if (numAnims >= 2)
		m_animations.reserve_and_fill(file, numAnims);

	if (m_headerVersion >= 0x1100)
	{
		const uint32_t numScalars = file.read<uint32_t>();
		if (numScalars >= 2)
			m_scalars.reserve_and_fill(file, numScalars);
		file.read(m_baseValues);
	}
}

void Model_Setup::save(FileWriter& file) const
{
	file.writeTag("GMPX");
	file << m_headerVersion;

	static constexpr char ZERO[12]{};
	static char garbo[16];
	file << ZERO << garbo;

	m_positions.write_size(file);
	m_rotations.write_size(file);
	m_positions.write_data(file);
	m_rotations.write_data(file);

	m_animations.write_size(file);
	if (m_animations.getSize() >= 2)
		m_animations.write_data(file);

	if (m_headerVersion >= 0x1100)
	{
		m_scalars.write_size(file);
		if (m_scalars.getSize() >= 2)
			m_scalars.write_data(file);
		file.write(m_baseValues);
	}
}
