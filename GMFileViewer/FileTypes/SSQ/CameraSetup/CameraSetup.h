#pragma once
#include <stdint.h>
#include "../Keyframe.h"
#include "LightSetup.h"

class CameraSetup
{
	struct BaseGlobalValues
	{
		glm::vec3 clearColor;
		float unknown_1f;
		glm::u32vec3 baseAmbience;
		unsigned long useDiffuse;
		glm::u32vec3 vertColorDiffuse;
		float fov;
		float aspectRatio;
		float zNear;
		float zFar;
		uint32_t unknown_2u;
	};

	struct Projection
	{
		float fov;
		float aspectRatio;
		float zNear;
		float zFar;
		uint32_t unknown;

		static Projection Mix(const Projection& current, const Projection& next, float coef)
		{
			return {
				::Mix(current.fov, next.fov, coef),
				::Mix(current.aspectRatio, next.aspectRatio, coef),
				::Mix(current.zNear, next.zNear, coef),
				::Mix(current.zFar, next.zFar, coef),
				current.unknown
			};
		}
	};

	union Val
	{
		char c[4];
		float f;
		unsigned long ul;
		long l;
	};

	struct Struct64_7f
	{
		uint32_t cameraRelated;
		Val ulong_b;
		Val ulong_c;
		Val ulong_d;
		Val ulong_e;
		char modelName[16];
		Val ulong_j;
		Val ulong_k;
		Val ulong_l;
		Val ulong_m;
	};

	BaseGlobalValues m_baseGlobalValues;
	KeyFrameArray<glm::vec3> m_positions;
	KeyFrameArray<glm::quat> m_rotations;
	KeyFrameArray<Projection> m_projections;
	KeyFrameArray<glm::vec3> m_ambientColors;
	GMArray<LightSetup> m_lights;
	KeyFrameArray<Struct64_7f> m_64bytes_v;

public:
	void read(FileReader& file);
	void save(FileWriter& file) const;
};
