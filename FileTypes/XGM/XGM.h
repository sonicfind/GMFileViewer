#pragma once
#include "IMX/IMX.h"
#include "XG/XG.h"
#include "XG/Animation.h"
#include <vector>

class XGM
{
	struct XGMNode
	{
		char m_filepath[256];
		char m_name[16];

		XGMNode(const char*& input, const uint32_t index);
	};

	struct XGMNode_IMX : public XGMNode
	{
		unsigned long m_non_model = 0;
		unsigned long m_unk = 0;
		IMX m_texture;

		XGMNode_IMX(const char*& input, const uint32_t index);
	};

	struct XGMNode_XG : public XGMNode
	{
		std::vector<Animation> m_animations;
		XG m_model;

		XGMNode_XG(const char*& input, const uint32_t index);
	};

	std::vector<XGMNode_IMX> m_textures;
	std::vector<XGMNode_XG> m_models;

public:
	XGM(const std::filesystem::path& filePath);
};
