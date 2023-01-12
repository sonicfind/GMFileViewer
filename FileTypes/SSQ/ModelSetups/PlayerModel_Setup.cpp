#include "PlayerModel_Setup.h"

PlayerModel_Setup::PlayerModel_Setup(const char*& input) : Model_Setup(input)
{
	if (m_headerVersion < 0x1300)
		return;

	const uint32_t numControllables = FileOps::Read<uint32_t>(input);
	m_controllables.reserve_and_fill(input, numControllables);
	m_connections.reserve(numControllables);
	for (auto& connection : m_connections)
		connection.reserve_and_fill(input);
	m_defaults.reserve_and_fill(input, numControllables);
}
