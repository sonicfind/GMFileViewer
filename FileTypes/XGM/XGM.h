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

		XGMNode(const char*& input);
		virtual void readData(const char*& input) = 0;
	};

	struct XGMNode_IMX : public XGMNode
	{
		unsigned long m_non_model = 0;
		unsigned long m_unk = 0;
		IMX m_texture;

		using XGMNode::XGMNode;
		void readData(const char*& input);
	};

	struct XGMNode_XG : public XGMNode
	{
		std::vector<Animation> m_animations;
		XG m_model;

		using XGMNode::XGMNode;
		void readData(const char*& input);
	};

	std::vector<XGMNode_IMX> m_textures;
	std::vector<XGMNode_XG> m_models;

public:
	XGM(const std::filesystem::path& filePath);
};
