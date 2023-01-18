#pragma once
#include "../XG.h"
#include "XG_MaterialNode.h"
#include "xgTexture.h"

class xgMaterial : public XG_SubNode, public XG_MaterialNode
{
	uint32_t m_blendType = 0;
	uint32_t m_shadingType = 0;

	alignas(16) struct Diffuse
	{
		float red = 0;
		float green = 0;
		float blue = 0;
		float alpha = 1;
	} m_diffuse;

	struct Specular
	{
		float red = 0;
		float green = 0;
		float blue = 0;
		float exponent = 0;
	} m_specular;

	uint32_t m_flags = 0;
	uint32_t m_textureEnv = 0;
	uint32_t m_uTile = 0;
	uint32_t m_vTile = 0;

	xgTexture* m_inputTexture = nullptr;

public:
	void load(FilePointer& file, const XG* xg) override;
	void bind() const;
};

