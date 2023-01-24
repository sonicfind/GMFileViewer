#include "SSQ.h"
#include "ModelSetups/Model_Setup.h"
#include "ModelSetups/PlayerModel_Setup.h"
#include "ModelSetups/AttDefModel_Setup.h"
#include "ModelSetups/SnakeModel_Setup.h"
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
	{
		switch (m_xgEntries[i].getModelType())
		{
		case SSQModelType::Normal:
			m_models[i] = std::make_unique<Model_Setup>(file);
			break;
		case SSQModelType::Player1:
		case SSQModelType::Player2:
		case SSQModelType::DuetPlayer:
			m_models[i] = std::make_unique<PlayerModel_Setup>(file);
			break;
		case SSQModelType::Player1AttDef:
		case SSQModelType::Player2AttDef:
		case SSQModelType::DuetPlayerAttDef:
		case SSQModelType::DuetComboAttack:
			m_models[i] = std::make_unique<AttDefModel_Setup>(file);
			break;
		case SSQModelType::Snake:
			m_models[i] = std::make_unique<SnakeModel_Setup>(file);
			break;
		default:
			throw "Bad modelSetup type";
		}
	}

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

void SSQ::update(float frame)
{
	for (size_t i = 0; i < m_matrices.getSize(); ++i)
		m_matrices[i] = m_models[i]->getModelMatrix(frame);


}
