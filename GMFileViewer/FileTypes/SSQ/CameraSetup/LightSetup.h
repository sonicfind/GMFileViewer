#pragma once
#include "FileReader.h"
#include "Graphics.h"
#include "../Keyframe.h"

struct LightColors
{
	glm::vec3 diffuse;
	glm::vec3 specular;

	static LightColors Mix(const LightColors& lhs, const LightColors& rhs, float frame)
	{
		return { glm::mix(lhs.diffuse, rhs.diffuse, frame), glm::mix(lhs.specular, rhs.specular, frame) };
	}
};

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
		LightColors colors;
		glm::quat rotation;
		glm::vec3 buffer;
		float val;
		Val ulong_e;
		Val ulong_f;
		Val ulong_g;
		Val ulong_h;
		Val ulong_i;
	};

public:
	BaseValues m_baseValues;

private:
	uint32_t m_headerVersion = 0;
	KeyFrameArray<glm::quat> m_rotations;
	KeyFrameArray<LightColors> m_colors;

public:
	void read(FileReader& file);
	void save(FileWriter& file) const;
	void update(Graphics& gfx, float frame, uint32_t offset) const;

private:
	glm::quat getRotation(float frame) const;
	LightColors getColors(float frame) const;
};
