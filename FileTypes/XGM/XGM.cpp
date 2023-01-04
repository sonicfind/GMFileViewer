#include "XGM.h"

XGM::XGMNode::XGMNode(const char*& input)
{
	strncpy_s(m_filepath, input, sizeof(m_filepath));
	input += sizeof(m_filepath);

	strncpy_s(m_name, input, sizeof(m_name));
	input += sizeof(m_name);
}

void XGM::XGMNode_IMX::readData(const char*& input)
{
	const auto fileSize = FileOps::Read<uint32_t>(input);

	input += 4;
	FileOps::Read(m_non_model, input);
	FileOps::Read(m_unk, input);
	input += 12;

	m_texture.load(input);
	input += fileSize;
}

void XGM::XGMNode_XG::readData(const char*& input)
{
	const auto fileSize = FileOps::Read<uint32_t>(input);
	const auto numAnimations = FileOps::Read<uint32_t>(input);
	input += 4;

	m_animations.reserve(numAnimations);
	for (uint32_t i = 0; i < numAnimations; ++i)
	{
		m_animations.emplace_back(input);
		input += 32;
	}

	m_model.load(input);
	input += fileSize;
}

XGM::XGM(const std::filesystem::path& filePath)
{
	const FileOps::FilePointers file(filePath);
	auto input = file.begin();

	const auto numTextures = FileOps::Read<uint32_t>(input);
	const auto numModels = FileOps::Read<uint32_t>(input);

	try
	{
		m_textures.reserve(numTextures);
		for (uint32_t i = 0; i < numTextures; ++i)
		{
			m_textures.emplace_back(input);
			if (FileOps::Read<uint32_t>(input) != i)
				throw "weird af";

			m_textures.back().readData(input);
		}

		m_models.reserve(numModels);
		for (uint32_t i = 0; i < numModels; ++i)
		{
			m_models.emplace_back(input);
			if (FileOps::Read<uint32_t>(input) != i)
				throw "weird af";

			m_models.back().readData(input);
		}
	}
	catch (...)
	{

	}
}
