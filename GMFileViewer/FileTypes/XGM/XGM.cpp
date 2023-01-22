#include "XGM.h"
#include "XG/SubNodes/xgBgGeometry.h"
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
}

void XGM::createGraphicsBuffers(size_t index)
{
	for (auto& texture : m_textures)
		texture.createTextureBuffer();

	m_models[index].createVertexBuffers();
}

void XGM::testGraphics(size_t index)
{
	createGraphicsBuffers(index);

	const Graphics* gfx = Graphics::getGraphics();
	gfx->setClearColor(0.2f, 0.5f, 0.2f, 1.0f);
	DirectX::XMFLOAT3 pos = { 0.0f, 100.0f, -400.0f };
	DirectX::XMFLOAT3 front = { 0.0f, 100.0f, -399.0f };
	DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };

	auto view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&pos), DirectX::XMLoadFloat3(&front), DirectX::XMLoadFloat3(&up));
	auto combo = view * DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.f), 4.f / 3, 1.0f, 1800000.0f);

	gfx->bindConstantBuffer(Graphics::View);
	gfx->updateConstantBuffer(0, &view, sizeof(DirectX::XMMATRIX));

	gfx->bindConstantBuffer(Graphics::ComboViewAndProjection);
	gfx->updateConstantBuffer(0, &combo, sizeof(DirectX::XMMATRIX));

	gfx->enable(Graphics::Depth_Test);

	auto t1 = std::chrono::high_resolution_clock::now();
	size_t count = 0;
	float prev = 0;
	for (size_t i = 0; !gfx->shouldClose(); ++i)
	{
		auto t2 = std::chrono::high_resolution_clock::now();
		float time = 30 * std::chrono::duration<float>(t2 - t1).count();
		if (time >= prev + 30)
		{
			std::cout << 1000.0 / double(count) << " ms/frame\n";
			std::cout << count << " frames" << std::endl;
			count = 0;
			prev = int(time / 30) * 30;
		}
		++count;

		gfx->resetFrame();
		m_models[index].update(0, time, LoopControl::LOOP_ALL, PlaybackDirection::FORWARDS);
		m_models[index].draw(DirectX::XMMatrixIdentity());
		gfx->displayFrame();
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000 << " milliseconds\n";
}

void XGM::displayModelList() const
{
	for (uint32_t i = 0; i < m_models.getSize(); ++i)
		std::cout << i + 1 << " - " << m_models[i].m_name << "\n";
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

	if (control == LoopControl::LOOP_ANIM)
		frame = fmod(frame, m_animations[index].calcLength(120));
	else if (control != LoopControl::HALT)
	{
		while (index < m_animations.getSize() - 1 || control == LoopControl::LOOP_ALL)
		{
			const float length = m_animations[index].calcLength(120);
			if (frame < length || length == 0)
				break;

			frame -= length;
			if (index < m_animations.getSize() - 1)
				++index;
			else
				index = 0;
		}
	}

	float key = m_animations[index].getTimelinePosition(frame, 120, playbackDirection);
	m_model.update(key);
}

void XGM::XGMNode_XG::draw(const DirectX::XMMATRIX& modelMatrix) const
{
	m_model.draw(modelMatrix);
}
