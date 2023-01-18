#include "SSQ.h"
#include "FilePointer.h"

SSQ::SSQ(const std::filesystem::path& filePath)
{
	FilePointer file(filePath);

	if (!file.checkTag("GMSX"))
		throw "SSQ file read error";

	const uint32_t headerVersion = file.read<uint32_t>();
	file += 28;

	m_matrices.reserve(file);
	m_imxEntries.reserve_and_fill(file);
	m_xgEntries.reserve_and_fill(file);
	m_models.reserve(m_xgEntries.getSize());

	for (uint32_t i = 0; i < m_xgEntries.getSize(); ++i)
		m_models[i] = Model_Setup::create(file, m_xgEntries[i].getModelType());

	m_camera.read(file);
	m_sprites.read(file);

	if (headerVersion >= 0x1100)
	{
		m_textureAnims.reserve(file);
		for (auto& texAnim : m_textureAnims)
			texAnim.read(file);
	}

	if (headerVersion >= 0x1200)
		m_pSetup.read(file);
}
