#pragma once
#include "../SSQ/ModelSetups/PlayerModel_Setup.h"

class WEB
{
	std::unique_ptr<PlayerModel_Setup> m_setup;
public:
	WEB(std::filesystem::path& filePath);
};
