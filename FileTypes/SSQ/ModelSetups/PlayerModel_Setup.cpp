#include "PlayerModel_Setup.h"

PlayerModel_Setup::PlayerModel_Setup(FilePointer& file) : Model_Setup(file)
{
	if (m_headerVersion < 0x1300)
		return;

	const uint32_t numControllables = file.read<uint32_t>();
	m_controllables.reserve_and_fill(file, numControllables);
	m_connections.reserve(numControllables);
	for (auto& connection : m_connections)
		connection.reserve_and_fill(file);
	m_defaults.reserve_and_fill(file, numControllables);
}

PlayerModel_Setup::PlayerModel_Setup(FilePointer&& file) : PlayerModel_Setup(file) {}
