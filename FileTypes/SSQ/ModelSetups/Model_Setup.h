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
		DirectX::XMFLOAT3 scalar;
		unsigned long envMap_maybe;
		static ModelScalar mix(const ModelScalar& current, const ModelScalar& next, float coef)
		{
			ModelScalar result;
			DirectX::XMStoreFloat3(&result.scalar, DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&current.scalar), DirectX::XMLoadFloat3(&next.scalar), coef));
			result.envMap_maybe = current.envMap_maybe;
			return result;
		}
	};

	struct BaseValues
	{
		DirectX::XMFLOAT3 basePosition;
		DirectX::XMFLOAT4 baseRotation;
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
