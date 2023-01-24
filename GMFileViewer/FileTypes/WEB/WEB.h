#pragma once
#include "../SSQ/ModelSetups/PlayerModel_Setup.h"

class WEB
{
	std::unique_ptr<PlayerModel_Setup> m_setup;
public:
	WEB(const std::filesystem::path& filePath);
};
