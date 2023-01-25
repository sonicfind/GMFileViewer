#pragma once
#include "FileReader.h"
#include "../Keyframe.h"

class LightSetup
{
	union Val
	{
		char c[4];
		float f;
		unsigned long ul;
		long l;
	};

	struct BaseValues
	{
		unsigned long isActive;
		DirectX::XMFLOAT3 diffuse;
		DirectX::XMFLOAT3 specular;
		DirectX::XMFLOAT4 rotation;
		Val ulong_b;
		Val l_a;
		Val ulong_c;
		Val ulong_d;
		Val ulong_e;
		Val ulong_f;
		Val ulong_g;
		Val ulong_h;
		Val ulong_i;
	};

	struct LightColors
	{
		DirectX::XMFLOAT3 diffuse;
		DirectX::XMFLOAT3 specular;
	};

	BaseValues m_baseValues;
	uint32_t m_headerVersion = 0;
	RotationKeyframes m_rotations;
	KeyFrameArray<LightColors> m_colors;

public:
	void read(FileReader& file);
	void save(FileWriter& file) const;
};
