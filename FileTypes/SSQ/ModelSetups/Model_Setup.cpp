#include "Model_Setup.h"
#include "FileOperations.h"

Model_Setup::Model_Setup(const char*& input)
{
	if (!FileOps::checkTag("GMPX", input))
		throw "Model Setup read error";

	FileOps::Read(m_headerVersion, input);
	input += 28;

	m_positions.reserve(input);
	m_rotations.reserve(input);
	m_positions.fill(input);
	m_rotations.fill(input);

	const uint32_t numAnims = FileOps::Read<uint32_t>(input);
	if (numAnims >= 2)
		m_animations.reserve_and_fill(input, numAnims);

	if (m_headerVersion >= 0x1100)
	{
		const uint32_t numScalars = FileOps::Read<uint32_t>(input);
		if (numScalars >= 2)
			m_scalars.reserve_and_fill(input, numScalars);
		FileOps::Read(m_baseValues, input);
	}
}
