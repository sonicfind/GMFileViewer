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

		uint32_t load(FilePointer& file, const uint32_t index);
	};

	struct XGMNode_IMX : public XGMNode
	{
		unsigned long m_non_model = 0;
		unsigned long m_unk = 0;
		IMX m_texture;

		void load(FilePointer& file, const uint32_t index);
	};

	struct XGMNode_XG : public XGMNode
	{
		GMArray<Animation> m_animations;
		XG m_model;

		void load(FilePointer& file, const uint32_t index);
	};

	GMArray<XGMNode_IMX> m_textures;
	GMArray<XGMNode_XG> m_models;

public:
	XGM(const std::filesystem::path& filePath);
};
