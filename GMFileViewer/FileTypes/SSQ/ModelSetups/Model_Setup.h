#pragma once
#include "FileReader.h"
#include "../Keyframe.h"
#include "../SSQModelType.h"
#include "XGM/XG/AnimControl.h"

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
	};

	struct ModelScalar
	{
		glm::vec3 scalar;
		unsigned long envMap_maybe;
		static ModelScalar Mix(const ModelScalar& current, const ModelScalar& next, float coef)
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
		glm::quat baseRotation;
		unsigned long baseAnimIndex_maybe = 0;
		unsigned long ulong_b = 0;
		unsigned long depthTest = 1;
		unsigned long ulong_d = 0;
		unsigned long ulong_e = 0;
		unsigned long ulong_f = 0;
		float float_h = 0;
		float float_i = 0;
		unsigned long ulong_g = 0;
	};

	
	KeyFrameArray<glm::vec3> m_positions;
	KeyFrameArray<glm::quat> m_rotations;
	KeyFrameArray<ModelAnim> m_animations;
	KeyFrameArray<ModelScalar> m_scalars;
	BaseValues m_baseValues;

public:
	Model_Setup(FileReader& file);
	virtual void save(FileWriter& file) const;
	virtual ~Model_Setup() = default;
	
	glm::mat4 getModelMatrix(float frame) const;

	struct AnimProperties
	{
		ModelDrawStatus drawStatus;
		uint32_t animIndex;
		float frame;
		LoopControl control;
		PlaybackDirection direction;
		bool depthTest;
	};
	AnimProperties getAnimProperties(float frame) const;

private:
	virtual AnimProperties getAnimFromGamestate(float frame) const;

	glm::vec3 getScalar(float frame) const;
	glm::quat getRotation(float frame) const;
	glm::vec3 getTranslation(float frame) const;

};
