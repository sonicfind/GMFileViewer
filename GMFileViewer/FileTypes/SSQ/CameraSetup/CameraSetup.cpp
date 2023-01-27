#include "CameraSetup.h"
#include "Graphics.h"

void CameraSetup::read(FileReader& file)
{
	if (!file.checkTag("GMPX"))
		throw "Camera Setup read error";

	const uint32_t headerVersion = file.read<uint32_t>();

	file += 28;
	file.read(m_baseGlobalValues);
	m_positions.reserve(file);
	m_rotations.reserve(file);
	m_positions.fill(file);
	m_rotations.fill(file);

	const uint32_t numProjections = file.read<uint32_t>();
	if (numProjections >= 2)
		m_projections.reserve_and_fill(file, numProjections);

	const uint32_t numColors = file.read<uint32_t>();
	if (numColors >= 2)
		m_ambientColors.reserve_and_fill(file, numColors);

	m_lights.reserve(file);
	for (auto& light : m_lights)
	{
		file.read(light.m_baseValues);

		if (light.m_baseValues.isActive)
			light.read(file);
	}

	if (headerVersion >= 0x1200)
	{
		const uint32_t numUnknown = file.read<uint32_t>();
		if (numUnknown >= 2)
			m_64bytes_v.reserve_and_fill(file, numUnknown);
	}
}

void CameraSetup::save(FileWriter& file) const
{
	file.writeTag("GMPX");
	file << 0x1100 + 0x100 * (m_64bytes_v.getSize() >= 2);

	static constexpr char ZERO[12]{};
	static char garbo[16];
	file << ZERO << garbo;

	file.write(m_baseGlobalValues);

	m_positions.write_size(file);
	m_rotations.write_size(file);
	m_positions.write_data(file);
	m_rotations.write_data(file);

	m_projections.write_size(file);
	if (m_projections.getSize() >= 2)
		m_projections.write_data(file);

	m_ambientColors.write_size(file);
	if (m_ambientColors.getSize() >= 2)
		m_ambientColors.write_data(file);

	m_lights.write_size(file);
	for (const auto& light : m_lights)
	{
		file.write(light.m_baseValues);

		if (light.m_baseValues.isActive)
			light.save(file);
	}

	if (m_64bytes_v.getSize() >= 2)
		m_64bytes_v.write_full(file);
}

void CameraSetup::update(float frame) const
{
	Graphics* gfx = Graphics::getGraphics();
	auto view = calcViewMatrix(frame);
	gfx->bindConstantBuffer(Graphics::View);
	gfx->updateConstantBuffer(0, &view, sizeof(glm::mat4));

	auto combo = calcProjectionMatrix(frame) * view;
	gfx->bindConstantBuffer(Graphics::ComboViewAndProjection);
	gfx->updateConstantBuffer(0, &combo, sizeof(glm::mat4));
}

glm::mat4 CameraSetup::calcProjectionMatrix(float frame) const
{
	if (m_projections.isEmpty())
		return glm::perspective(glm::radians(m_baseGlobalValues.fov), m_baseGlobalValues.aspectRatio, m_baseGlobalValues.zNear, m_baseGlobalValues.zFar);

	const auto projection = InterpolateStruct(m_projections, frame);
	return glm::perspective(glm::radians(projection.fov), projection.aspectRatio, projection.zNear, projection.zFar);
}

glm::mat4 CameraSetup::calcViewMatrix(float frame) const
{
	auto position = Interpolate(m_positions, frame);
	position.z *= -1;

	const auto rotation = Interpolate(m_rotations, frame);
	glm::mat4 view = glm::toMat4(rotation) * glm::lookAt(position, position + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	view[2] *= -1.f;
	return view;
}
