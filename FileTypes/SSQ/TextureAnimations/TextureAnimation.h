#pragma once
#include "../Keyframe.h"
class TexAnim
{
	struct CutOut
	{
		glm::vec2 topLeft;
		glm::vec2 bottomRight;
	};

	struct TexFrame
	{
		uint32_t cutOutIndex;
	};

	glm::u32vec2 m_offset;
	char m_textureName[16];
	GMArray<CutOut> m_cutOuts;
	GMArray<Keyframe<TexFrame>> m_cutoutTimeline;
public:
	void read(const char*& input);
};
