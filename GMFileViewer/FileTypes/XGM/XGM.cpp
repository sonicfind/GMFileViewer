#include "XGM.h"
#include "TaskQueue.h"
#include "Graphics.h"
#include <iostream>

XGM::XGM(const std::filesystem::path& filePath)
{
	FilePointer file(filePath);

	m_textures.reserve(file);
	m_models.reserve(file);

	for (uint32_t i = 0; i < m_textures.getSize(); ++i)
		m_textures[i].load(file, i);

	for (uint32_t i = 0; i < m_models.getSize(); ++i)
		m_models[i].load(file, i);

	TaskQueue::getInstance().waitForCompletedTasks();

	Graphics::initGraphics(Graphics::Backend::DirectX);

	auto t1 = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 10000; ++i)
		for (const auto& model : m_models)
		{
			model.update(0, 12, LoopControl::NORMAL, PlaybackDirection::FORWARDS);
			model.draw(DirectX::XMMatrixIdentity());
		}
	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000 << " milliseconds\n";
}

uint32_t XGM::XGMNode::load(FilePointer& file, const uint32_t index)
{
	file.read(m_filepath);
	file.read(m_name);

	if (file.read<uint32_t>() != index)
		throw "weird af";

	return file.read<uint32_t>();
}

void XGM::XGMNode_IMX::load(FilePointer& file, const uint32_t index)
{
	const uint32_t fileSize = XGMNode::load(file, index);

	file += 4;
	file.read(m_non_model);
	file.read(m_unk);
	file += 12;

	TaskQueue::getInstance().addTask([this, file] { m_texture.load(file); });
	file += fileSize;
}

void XGM::XGMNode_IMX::createTextureBuffer()
{
	m_texture.createTextureBuffer(m_name);
}

void XGM::XGMNode_XG::load(FilePointer& file, const uint32_t index)
{
	const uint32_t fileSize = XGMNode::load(file, index);

	m_animations.reserve(file);
	file += 4;
	m_animations.fill(file);
	
	TaskQueue::getInstance().addTask([this, file] { m_model.load(file); });
	file += fileSize;
}

void XGM::XGMNode_XG::createVertexBuffers()
{
	m_model.createVertexBuffers();
}

void XGM::XGMNode_XG::update(uint32_t index, float frame, LoopControl control, PlaybackDirection playbackDirection) const
{
	if (index >= m_animations.getSize())
		index = m_animations.getSize() - 1;

	if (control == LoopControl::LOOP)
		frame = fmod(frame, m_animations[index].calcLength(120));
	else if (control == LoopControl::NORMAL)
	{
		while (index < m_animations.getSize() - 1)
		{
			const float length = m_animations[index].calcLength(120);
			if (frame < length)
				break;

			frame -= length;
			++index;
		}
	}

	float key = m_animations[index].getTimelinePosition(frame, 120, playbackDirection);
	m_model.update(key);
}

void XGM::XGMNode_XG::draw(const DirectX::XMMATRIX& modelMatrix) const
{
	m_model.draw(modelMatrix);
}
