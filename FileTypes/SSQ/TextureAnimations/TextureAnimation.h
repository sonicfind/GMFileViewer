#pragma once
#include "../Keyframe.h"
class TexAnim
{
	struct CutOut
	{
		DirectX::XMFLOAT2 topLeft;
		DirectX::XMFLOAT2 bottomRight;
	};

	struct TexFrame
	{
		uint32_t cutOutIndex;
	};

	DirectX::XMUINT2 m_offset;
	char m_textureName[16];
	GMArray<CutOut> m_cutOuts;
	KeyFrameArray<TexFrame> m_cutoutTimeline;
public:
	void read(FilePointer& file);
};
