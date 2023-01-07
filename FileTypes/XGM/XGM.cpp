#include "XGM.h"
#include "TaskQueue.h"

XGM::XGM(const std::filesystem::path& filePath)
{
	const FileOps::FilePointers file(filePath);
	auto input = file.begin();

	m_textures.reserve(input);
	m_models.reserve(input);

	try
	{
		for (uint32_t i = 0; i < m_textures.getSize(); ++i)
			m_textures[i].load(input, i);

		for (uint32_t i = 0; i < m_models.getSize(); ++i)
			m_models[i].load(input, i);
	}
	catch (...)
	{
		
	}

	TaskQueue::getInstance().waitForCompletedTasks();
}

uint32_t XGM::XGMNode::load(const char*& input, const uint32_t index)
{
	FileOps::Read(m_filepath, input);
	FileOps::Read(m_name, input);

	if (FileOps::Read<uint32_t>(input) != index)
		throw "weird af";

	return FileOps::Read<uint32_t>(input);
}

void XGM::XGMNode_IMX::load(const char*& input, const uint32_t index)
{
	const uint32_t fileSize = XGMNode::load(input, index);

	input += 4;
	FileOps::Read(m_non_model, input);
	FileOps::Read(m_unk, input);
	input += 12;

	TaskQueue::getInstance().addTask([this, input] { m_texture.load(input); });
	input += fileSize;
}

void XGM::XGMNode_XG::load(const char*& input, const uint32_t index)
{
	const uint32_t fileSize = XGMNode::load(input, index);

	m_animations.reserve(input);
	input += 4;
	m_animations.fill(input);
	
	TaskQueue::getInstance().addTask([this, input] { m_model.load(input); });
	input += fileSize;
}
