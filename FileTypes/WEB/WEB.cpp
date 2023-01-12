#include "WEB.h"

WEB::WEB(std::filesystem::path& filePath)
{
	const FileOps::FilePointers file(filePath);
	auto input = file.begin();
	m_setup = std::unique_ptr<PlayerModel_Setup>(new PlayerModel_Setup(input));
}
