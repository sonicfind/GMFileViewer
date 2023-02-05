#pragma once
#include "IMX/IMX.h"
#include "XG/XG.h"
#include "XG/Animation.h"

class XGM
{
	struct XGMNode
	{
		char m_filepath[256];
		char m_name[16];

	protected:
		uint32_t load(FileReader& file, const uint32_t index);
		std::streampos save(FileWriter& file, const uint32_t index) const;
	};

	struct XGMNode_IMX : public XGMNode
	{
		unsigned long m_non_model = 0;
		unsigned long m_unk = 0;
		IMX m_texture;

		void load(FileReader& file, const uint32_t index);
		uint32_t save(FileWriter& file, const uint32_t index, uint32_t totalSizes) const;
		void createTextureBuffer();
	};
	
	struct XGMNode_XG : public XGMNode
	{
		GMArray<Animation> m_animations;
		XG m_model;

		void load(FileReader& file, const uint32_t index);
		void save(FileWriter& file, const uint32_t index) const;

		void createVertexBuffers();
		void addInstance();
		void update(uint32_t instance, const glm::mat4& modelMatrix, uint32_t index, float frame, LoopControl control, PlaybackDirection playbackDirection);
		void draw(uint32_t instance, bool doTransparentMeshes) const;
	};

	GMArray<XGMNode_IMX> m_textures;
	GMArray<XGMNode_XG> m_models;

public:
	XGM(const std::filesystem::path& filePath);
	void saveToFile(const std::filesystem::path& filePath) const;
	void displayModelList() const;
	uint32_t getNumModels() const { return m_models.getSize(); }

	void testGraphics(size_t index);

	void initTextureBuffers();
	void initModelBuffer(std::string_view modelName);
	void addInstanceToModel(std::string_view modelName);
	void updateModel(std::string_view modelName, uint32_t instance, const glm::mat4& modelMatrix, uint32_t animIndex, float frame, LoopControl control, PlaybackDirection playbackDirection);
	void drawModel(std::string_view modelName, uint32_t instance, bool doTransparentMeshes) const;
	void mixedUpdateAndDrawModel(std::string_view modelName, uint32_t instance, const glm::mat4& modelMatrix, uint32_t animIndex, float frame, LoopControl control, PlaybackDirection playbackDirection);

private:
	XGMNode_XG& getModel(std::string_view modelName);
	const XGMNode_XG& getModel(std::string_view modelName) const;
};
