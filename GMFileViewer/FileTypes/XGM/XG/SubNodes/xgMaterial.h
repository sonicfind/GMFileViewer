#pragma once
#include "XG_MaterialNode.h"
#include "xgTexture.h"
#include "Graphics.h"

class xgMaterial : public XG_MaterialNode
{
	enum Shading : uint32_t
	{
		Unshaded,
		Shaded_unk,
		Shaded,
		VertexColors,
		VertexColors_Shaded,
	};

	Blending m_blendType = Blending::Opaque;
	Shading m_shadingType = Shading::Unshaded;
	uint32_t :32, :32;

	struct Diffuse
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
	using XG_MaterialNode::XG_MaterialNode;
	void load(FileReader& file, const XG* xg) override;

	void writeType(FileWriter& file) const override;
	void save(FileWriter& file) const override;

	void bind(size_t slot) const override;
	size_t getNumMaterials() const override;
};

