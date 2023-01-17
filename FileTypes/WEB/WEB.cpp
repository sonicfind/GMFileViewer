#include "WEB.h"

WEB::WEB(std::filesystem::path& filePath) : m_setup(new PlayerModel_Setup(FilePointer(filePath))) {}
