#include "Model_Setup.h"
#include "FileReader.h"
#include <glm/gtx/quaternion.hpp>

Model_Setup::Model_Setup(FileReader& file)
{
	if (!file.checkTag("GMPX"))
		throw "Model Setup read error";

	file.read(m_headerVersion);
	file += 28;

	m_positions.reserve(file);
	m_rotations.reserve(file);
	m_positions.fill(file);
	m_rotations.fill(file);

	const uint32_t numAnims = file.read<uint32_t>();
	if (numAnims >= 2)
		m_animations.reserve_and_fill(file, numAnims);

	if (m_headerVersion >= 0x1100)
	{
		const uint32_t numScalars = file.read<uint32_t>();
		if (numScalars >= 2)
			m_scalars.reserve_and_fill(file, numScalars);
		file.read(m_baseValues);
	}
}

void Model_Setup::save(FileWriter& file) const
{
	file.writeTag("GMPX");
	file << m_headerVersion;

	static constexpr char ZERO[12]{};
	static char garbo[16];
	file << ZERO << garbo;

	m_positions.write_size(file);
	m_rotations.write_size(file);
	m_positions.write_data(file);
	m_rotations.write_data(file);

	m_animations.write_size(file);
	if (m_animations.getSize() >= 2)
		m_animations.write_data(file);

	if (m_headerVersion >= 0x1100)
	{
		m_scalars.write_size(file);
		if (m_scalars.getSize() >= 2)
			m_scalars.write_data(file);
		file.write(m_baseValues);
	}
}

bool Model_Setup::usesPriorDepthForTransparency() const
{
	return m_baseValues.depthTest;
}

glm::mat4 Model_Setup::getModelMatrix(float frame) const
{
	glm::mat4 matrix = glm::toMat4(getRotation(frame));
	matrix[3] = glm::vec4(getTranslation(frame), 1);

	const glm::vec scale = getScalar(frame);
	matrix[0] *= scale.x;
	matrix[1] *= scale.y;
	matrix[2] *= scale.z;
	return matrix;
}

Model_Setup::AnimProperties Model_Setup::getAnimProperties(float frame) const
{
	if (m_animations.isEmpty())
		return {
			ModelDrawStatus::Draw,
			m_baseValues.baseAnimIndex_maybe,
			frame, 
			LoopControl::LOOP_ANIM,
			PlaybackDirection::FORWARDS,
		};

	auto iter = Iterate(m_animations, frame);
	if (iter->object.noDrawing)
		return { ModelDrawStatus::NoDraw };
	else if (iter->object.pollGameState)
		return getAnimFromGamestate(frame);

	float start = iter->time;
	for (const Keyframe<ModelAnim>* key = iter - 1; key >= m_animations.begin() && !(key + 1)->object.startOverride && key->object.animIndex == iter->object.animIndex;)
	{
		start = key->time;
		--key;
	}

	LoopControl control;
	if (iter->object.holdLastFrame)
		control = LoopControl::HALT;
	else if (iter->object.loop)
		control = LoopControl::LOOP_ANIM;
	else
		control = LoopControl::NORMAL;

	return {
			iter->object.dropShadow ? ModelDrawStatus::Draw_withShadow : ModelDrawStatus::Draw,
			iter->object.animIndex,
			frame - start,
			control,
			PlaybackDirection::FORWARDS,
	};
}

Model_Setup::AnimProperties Model_Setup::getAnimFromGamestate(float frame) const
{
	return {
		ModelDrawStatus::Draw,
		m_baseValues.baseAnimIndex_maybe,
		frame,
		LoopControl::LOOP_ANIM,
		PlaybackDirection::FORWARDS,
	};
}

glm::vec3 Model_Setup::getScalar(float frame) const
{
	if (m_scalars.isEmpty())
		return { 1,1,1 };

	const auto key = InterpolateStruct(m_scalars, frame);
	return key.scalar;
}

glm::quat Model_Setup::getRotation(float frame) const
{
	glm::quat rotation = !m_rotations.isEmpty() ? Interpolate(m_rotations, frame) : m_baseValues.baseRotation;
	float tmp = rotation.z;
	rotation.z = rotation.y;
	rotation.y = tmp;
	return rotation;
}

glm::vec3 Model_Setup::getTranslation(float frame) const
{
	return !m_positions.isEmpty() ? Interpolate(m_positions, frame) : m_baseValues.basePosition;
}
