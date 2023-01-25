#include "SSQ.h"
#include "FileReader.h"
#include <iostream>

SSQ::SSQ(const std::filesystem::path& filePath)
{
	FileReader file(filePath);

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

		if (headerVersion >= 0x1200)
			m_pSetup.read(file);
	}
}

void SSQ::saveToFile(const std::filesystem::path& filePath) const
{
	FileWriter file(filePath);
	if (!file.isOpen())
	{
		std::cout << "File could not be opened for writing.\n";
		return;
	}

	file.writeTag("GMSX");
	file << uint32_t(0x1000 + 0x100 * !m_textureAnims.isEmpty());
	static constexpr char ZERO[12]{};
	static char garbo[16];
	file << ZERO << garbo;

	m_matrices.write_size(file);
	m_imxEntries.write_full(file);
	m_xgEntries.write_full(file);

	for (const auto& model : m_models)
		model->save(file);

	m_camera.save(file);
	m_sprites.save(file);

	if (!m_textureAnims.isEmpty())
	{
		m_textureAnims.write_size(file);
		for (const auto& texAnim : m_textureAnims)
			texAnim.save(file);
	}
}
