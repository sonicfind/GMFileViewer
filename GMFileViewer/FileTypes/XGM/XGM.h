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
		uint32_t load(FilePointer& file, const uint32_t index);
		void save(FileWriter& file, const uint32_t index) const;
	};

	struct XGMNode_IMX : public XGMNode
	{
		unsigned long m_non_model = 0;
		unsigned long m_unk = 0;
		IMX m_texture;

		void load(FilePointer& file, const uint32_t index);
		uint32_t save(FileWriter& file, const uint32_t index, uint32_t totalSizes) const;
		void createTextureBuffer();
	};
	
	struct XGMNode_XG : public XGMNode
	{
		GMArray<Animation> m_animations;
		XG m_model;

		void load(FilePointer& file, const uint32_t index);
		void save(FileWriter& file, const uint32_t index) const;

		void createVertexBuffers();
		void update(uint32_t index, float frame, LoopControl control, PlaybackDirection playbackDirection) const;
		void draw(const DirectX::XMMATRIX& modelMatrix) const;
	};

	GMArray<XGMNode_IMX> m_textures;
	GMArray<XGMNode_XG> m_models;

public:
	XGM(const std::filesystem::path& filePath);
	void saveToFile(const std::filesystem::path& filePath) const;
	void testGraphics(size_t index);
	void displayModelList() const;
	uint32_t getNumModels() const { return m_models.getSize(); }

private:
	void createGraphicsBuffers(size_t index);
};
