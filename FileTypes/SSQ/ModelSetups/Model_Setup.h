#pragma once
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
			return { glm::mix(current.scalar, next.scalar, coef), current.envMap_maybe };
		}
	};

	struct BaseValues
	{
		glm::vec3 basePosition;
		glm::quat baseRotation;
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
	Model_Setup(const char*& input);

public:
	virtual ~Model_Setup() = default;

	static std::unique_ptr<Model_Setup> create(const char*& input, SSQModelType type);
};
