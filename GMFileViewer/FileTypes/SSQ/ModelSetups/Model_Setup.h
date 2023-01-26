#pragma once
#include "FileReader.h"
#include "../Keyframe.h"
#include "../SSQModelType.h"

class Model_Setup
{
protected:
	uint32_t m_headerVersion = 0;

private:
	struct ModelAnim
	{
		unsigned long animIndex;
		unsigned long startOverride;
		unsigned long noDrawing;
		unsigned long pollGameState;
		unsigned long loop;
		unsigned long ulong_f;
		unsigned long holdLastFrame;
		unsigned long dropShadow;
		static ModelAnim mix(const ModelAnim& current, const ModelAnim& next, float coef)
		{
			return current;
		}
	};

	struct ModelScalar
	{
		glm::vec3 scalar;
		unsigned long envMap_maybe;
		static ModelScalar mix(const ModelScalar& current, const ModelScalar& next, float coef)
		{
			ModelScalar result;
			result.scalar = glm::mix(current.scalar, next.scalar, coef);
			result.envMap_maybe = current.envMap_maybe;
			return result;
		}
	};

	struct BaseValues
	{
		glm::vec3 basePosition;
		glm::vec4 baseRotation;
		unsigned long baseAnimIndex_maybe;
		unsigned long ulong_b;
		unsigned long depthTest;
		unsigned long ulong_d;
		unsigned long ulong_e;
		unsigned long ulong_f;
		float float_h;
		float float_i;
		unsigned long ulong_g;
	};

	
	PositionKeyframes m_positions;
	RotationKeyframes m_rotations;
	KeyFrameArray<ModelAnim> m_animations;
	KeyFrameArray<ModelScalar> m_scalars;
	BaseValues m_baseValues;

protected:
	Model_Setup(FileReader& file);

public:
	virtual void save(FileWriter& file) const;
	virtual ~Model_Setup() = default;

	static std::unique_ptr<Model_Setup> create(FileReader& file, SSQModelType type);
};
