#pragma once
#include "../Keyframe.h"

class FixedSprite
{
	// 
	struct Struct64_7f
	{
		uint32_t IMXEntryIndex;
		uint32_t ulong_b;
		uint32_t ulong_c;
		uint32_t ulong_d;
		uint32_t transparent;
		uint32_t ulong_f;
		uint32_t colorAlgorithm_maybe;
		uint32_t ulong_h;
		uint32_t blendingType;
		uint32_t mipmapDepth;
		uint32_t ulong_k;
		uint32_t ulong_l;
		uint32_t ulong_m;
		uint32_t ulong_n;
		uint32_t ulong_o;
		uint32_t ulong_p;
	};

	struct SpriteWorldValues
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 worldSize;
		uint32_t noDrawing;
		uint32_t ulong_b;
	};

	struct ColorMultipliers
	{
		DirectX::XMFLOAT4 colors;
		uint32_t ulong_a;
	};

	struct SpriteFrame
	{
		DirectX::XMFLOAT2 initial_BottomLeft;
		DirectX::XMFLOAT2 boxSize;
	};

	Struct64_7f m_64bytes;
	KeyFrameArray<SpriteWorldValues> m_worldValues;
	KeyFrameArray<ColorMultipliers> m_colors;
	KeyFrameArray<SpriteFrame> m_spriteFrames;

public:
	void read(FilePointer& file);
	void save(FileWriter& file) const;
};
