#pragma once
#include "FileReader.h"
#include "../Keyframe.h"

struct LightColors
{
	glm::vec3 diffuse;
	glm::vec3 specular;
};

template<>
struct Keyframe<LightColors>
{
	float time;
	float coefficient;
	LightColors object;
	InterpolationToggle interpolation;
	int32_t min;
	float coef;
	int32_t max;

	friend bool operator<(float currFrame, const Keyframe& keyframe)
	{
		return currFrame < keyframe.time;
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
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec4 rotation;
		Val ulong_b;
		Val l_a;
		Val ulong_c;
		Val ulong_d; // Usually 0
		Val ulong_e;
		Val ulong_f;
		Val ulong_g;
		Val ulong_h;
		Val ulong_i;
	};

	BaseValues m_baseValues;
	uint32_t m_headerVersion = 0;
	KeyFrameArray<glm::quat> m_rotations;
	KeyFrameArray<LightColors> m_colors;

public:
	void read(FileReader& file);
	void save(FileWriter& file) const;
};
