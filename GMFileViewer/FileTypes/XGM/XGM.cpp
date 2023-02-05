#include "XGM.h"
#include "XG/SubNodes/xgBgGeometry.h"
#include "TaskQueue.h"
#include "Graphics.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

XGM::XGM(const std::filesystem::path& filePath)
{
	FileReader file(filePath);

	m_textures.reserve(file);
	m_models.reserve(file);

	for (uint32_t i = 0; i < m_textures.getSize(); ++i)
		m_textures[i].load(file, i);

	for (uint32_t i = 0; i < m_models.getSize(); ++i)
		m_models[i].load(file, i);

	TaskQueue::getInstance().waitForCompletedTasks();
}

void XGM::saveToFile(const std::filesystem::path& filePath) const
{
	FileWriter file(filePath);
	if (!file.isOpen())
	{
		std::cout << "File could not be opened for writing.\n";
		return;
	}
	
	m_textures.write_size(file);
	m_models.write_size(file);

	uint32_t total = 0;
	for (uint32_t i = 0; i < m_textures.getSize(); ++i)
		total = m_textures[i].save(file, i, total);

	for (uint32_t i = 0; i < m_models.getSize(); ++i)
		m_models[i].save(file, i);
}

void XGM::initTextureBuffers()
{
	for (auto& texture : m_textures)
		texture.createTextureBuffer();
}

void XGM::initModelBuffer(std::string_view modelName)
{
	getModel(modelName).createVertexBuffers();
}

void XGM::addInstanceToModel(std::string_view modelName)
{
	getModel(modelName).addInstance();
}

void XGM::updateModel(std::string_view modelName, uint32_t instance, const glm::mat4& modelMatrix, uint32_t animIndex, float frame, LoopControl control, PlaybackDirection playbackDirection)
{
	getModel(modelName).update(instance, modelMatrix, animIndex, frame, control, playbackDirection);
}

void XGM::drawModel(std::string_view modelName, uint32_t instance, bool doTransparentMeshes) const
{
	getModel(modelName).draw(instance, doTransparentMeshes);
}

void XGM::mixedUpdateAndDrawModel(std::string_view modelName, uint32_t instance, const glm::mat4& modelMatrix, uint32_t animIndex, float frame, LoopControl control, PlaybackDirection playbackDirection)
{
	auto& model = getModel(modelName);
	model.update(instance, modelMatrix, animIndex, frame, control, playbackDirection);
	model.draw(instance, false);
	model.draw(instance, true);
}

void XGM::testGraphics(size_t index)
{
	Graphics::initGraphics(Graphics::Backend::OpenGL);
	initTextureBuffers();

	m_models[index].createVertexBuffers();
	Graphics::getGraphics()->updateTitle(m_models[index].m_name);

	const GraphicsInstance gfx = Graphics::getGraphics();
	gfx->setClearColor(0.2f, 0.5f, 0.2f, 1.0f);
	glm::vec3 pos = { 0.0f, 00.0f, 400.0f };
	glm::vec3 front = { 0.0f, 00.0f, 401.0f };
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };

	auto view = glm::lookAtLH(pos, front, up);
	view[2] *= -1.f;
	auto combo = glm::perspective(glm::radians(45.f), 4.f / 3, 1.0f, 1800000.0f) * view;

	gfx->bindConstantBuffer(Graphics::View);
	gfx->updateConstantBuffer(0, &view, sizeof(glm::mat4));

	gfx->bindConstantBuffer(Graphics::ComboViewAndProjection);
	gfx->updateConstantBuffer(0, &combo, sizeof(glm::mat4));

	gfx->bindConstantBuffer(Graphics::GlobalShading);
	const uint32_t numLights = 1;
	const glm::vec3 ambience = { 1, 1, 1 };
	gfx->updateConstantBuffer(0, &numLights, sizeof(glm::vec3));
	gfx->updateConstantBuffer(2 * sizeof(glm::vec4), &ambience, sizeof(glm::vec3));

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
			prev = int(time / 30) * 30.f;
		}
		++count;

		gfx->resetFrame();
		m_models[index].update(0, glm::identity<glm::mat4>(), 0, time, LoopControl::LOOP_ALL, PlaybackDirection::FORWARDS);
		m_models[index].draw(0, false);
		m_models[index].draw(0, true);
		gfx->displayFrame();
	}
	Graphics::closeGraphics();
}

void XGM::displayModelList() const
{
	for (uint32_t i = 0; i < m_models.getSize(); ++i)
		std::cout << i + 1 << " - " << m_models[i].m_name << "\n";
}

XGM::XGMNode_XG& XGM::getModel(std::string_view modelName)
{
	for (uint32_t i = 0; i < m_models.getSize(); ++i)
		if (modelName == m_models[i].m_name)
			return m_models[i];

	throw std::runtime_error("Name does not match any model");
}

const XGM::XGMNode_XG& XGM::getModel(std::string_view modelName) const
{
	for (uint32_t i = 0; i < m_models.getSize(); ++i)
		if (modelName == m_models[i].m_name)
			return m_models[i];

	throw std::runtime_error("Name does not match any model");
}

uint32_t XGM::XGMNode::load(FileReader& file, const uint32_t index)
{
	file.read(m_filepath);
	file.read(m_name);

	if (file.read<uint32_t>() != index)
		throw "weird af";

	return file.read<uint32_t>();
}

std::streampos XGM::XGMNode::save(FileWriter& file, const uint32_t index) const
{
	file.write(m_filepath, 256);
	file.write(m_name, 16);
	file << index;
	return file.tell();
}

void XGM::XGMNode_IMX::load(FileReader& file, const uint32_t index)
{
	const uint32_t fileSize = XGMNode::load(file, index);

	file += 4;
	file.read(m_non_model);
	file.read(m_unk);
	file += 12;

	TaskQueue::getInstance().addTask([this, file] { m_texture.load(file); });
	file += fileSize;
}

uint32_t XGM::XGMNode_IMX::save(FileWriter& file, const uint32_t index, uint32_t totalSizes) const
{
	const auto fileSizeLocation = XGMNode::save(file, index);
	uint32_t fileSize = 0;
	file << fileSize << totalSizes << m_non_model << m_unk;

	const char garbo[12]{};
	file.write(garbo, 12);

	const auto fileStart = file.tell();
	m_texture.save(file);
	const auto fileEnd = file.tell();

	fileSize = uint32_t(fileEnd - fileStart);
	file.seek(fileSizeLocation);
	file << fileSize;
	file.seek(fileEnd);

	return totalSizes + fileSize;
}

void XGM::XGMNode_IMX::createTextureBuffer()
{
	m_texture.createTextureBuffer(m_name);
}

void XGM::XGMNode_XG::load(FileReader& file, const uint32_t index)
{
	const uint32_t fileSize = XGMNode::load(file, index);

	m_animations.reserve(file);
	file += 4;
	m_animations.fill(file);
	
	TaskQueue::getInstance().addTask([this, file] { m_model.load(file); });
	file += fileSize;
}

void XGM::XGMNode_XG::save(FileWriter& file, const uint32_t index) const
{
	const auto fileSizeLocation = XGMNode::save(file, index);
	uint32_t fileSize = 0;
	file << fileSize;

	m_animations.write_size(file);
	file << uint32_t(0);
	m_animations.write_data(file);

	const auto fileStart = file.tell();
	m_model.save(file);
	const auto fileEnd = file.tell();

	fileSize = uint32_t(fileEnd - fileStart);
	file.seek(fileSizeLocation);
	file << fileSize;
	file.seek(fileEnd);
}

void XGM::XGMNode_XG::createVertexBuffers()
{
	m_model.createVertexBuffers();
}

void XGM::XGMNode_XG::addInstance()
{
	m_model.addInstance();
}

void XGM::XGMNode_XG::update(uint32_t instance, const glm::mat4& modelMatrix, uint32_t index, float frame, LoopControl control, PlaybackDirection playbackDirection)
{
	if (index >= m_animations.getSize())
		index = m_animations.getSize() - 1;

	if (control == LoopControl::LOOP_ALL)
	{
		do
		{
			const float length = m_animations[index].calcLength(65);
			if (frame < length || length == 0)
				break;

			frame -= length;
			if (index < m_animations.getSize() - 1)
				++index;
			else
				index = 0;
		} while (true);
	}

	float key = m_animations[index].getTimelinePosition(frame, 115, control, playbackDirection);
	m_model.update(instance, key, modelMatrix);
}

void XGM::XGMNode_XG::draw(uint32_t instance, bool doTransparentMeshes) const
{
	m_model.draw(instance, doTransparentMeshes);
}
