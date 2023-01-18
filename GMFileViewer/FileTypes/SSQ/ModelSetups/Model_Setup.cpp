#include "Model_Setup.h"
#include "FilePointer.h"

Model_Setup::Model_Setup(FilePointer& file)
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
