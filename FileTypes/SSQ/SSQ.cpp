#include "SSQ.h"
#include "FileOperations.h"

SSQ::SSQ(const std::filesystem::path& filePath)
{
	const FileOps::FilePointers file(filePath);
	auto input = file.begin();

	if (!FileOps::checkTag("GMSX", input))
		throw "SSQ file read error";

	const uint32_t headerVersion = FileOps::Read<uint32_t>(input);
	input += 28;

	m_matrices.reserve(input);
	m_imxEntries.reserve_and_fill(input);
	m_xgEntries.reserve_and_fill(input);
	m_models.reserve(m_xgEntries.getSize());

	for (uint32_t i = 0; i < m_xgEntries.getSize(); ++i)
		m_models[i] = Model_Setup::create(input, m_xgEntries[i].getModelType());

	m_camera.read(input);
	m_sprites.read(input);

	if (headerVersion >= 0x1100)
	{
		m_textureAnims.reserve(input);
		for (auto& texAnim : m_textureAnims)
			texAnim.read(input);
	}

	m_pSetup.read(input);
}
