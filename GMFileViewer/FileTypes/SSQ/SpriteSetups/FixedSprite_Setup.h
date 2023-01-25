#pragma once
#include "FixedSprite.h"
#include "../SSQModelType.h"
class FixedSpriteSetup
{
	struct Struct80_7f
	{
		unsigned long IMXindex;
		DirectX::XMFLOAT2 initial_BottomLeft;
		DirectX::XMFLOAT2 boxSize;
		DirectX::XMFLOAT3 worldPosition;
		DirectX::XMFLOAT2 worldSize;
		DirectX::XMFLOAT2 worldSize_ZW_maybe;
		float float_l;
		unsigned long ulong_b;
		float float_m;
		unsigned long ulong_c;
		unsigned long depthTest;
		SSQModelType modelTypeMapping = SSQModelType::Normal;
		unsigned long ulong_f;
		unsigned long ulong_g;
	};

	GMArray<Struct80_7f> m_80bytes;
	GMArray<FixedSprite> m_fixedSprites;

public:
	void read(FileReader& file);
	void save(FileWriter& file) const;

	uint32_t getNumSprites() const;
};
