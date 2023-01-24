#include "WEB.h"

WEB::WEB(const std::filesystem::path& filePath) : m_setup(new PlayerModel_Setup(FilePointer(filePath))) {}
