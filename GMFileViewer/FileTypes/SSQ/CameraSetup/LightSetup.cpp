#include "LightSetup.h"
#include "Graphics.h"

void LightSetup::read(FileReader& file)
{
	if (!file.checkTag("GMLT"))
		throw "Light Setup read error";

	file.read(m_headerVersion);
	file += 28;

	const uint32_t numRotations = file.read<uint32_t>();
	const uint32_t numColors = file.read<uint32_t>();
	if (numRotations >= 2)
		m_rotations.reserve_and_fill(file, numRotations);

	if (numColors >= 2)
		m_colors.reserve_and_fill(file, numColors);
}

void LightSetup::save(FileWriter& file) const
{
	file.writeTag("GMLT");
	file.write(m_headerVersion);

	static constexpr char ZERO[12]{};
	static char garbo[16];
	file << ZERO << garbo;

	m_rotations.write_size(file);
	m_colors.write_size(file);
	if (m_rotations.getSize() >= 2)
		m_rotations.write_data(file);

	if (m_colors.getSize() >= 2)
		m_colors.write_data(file);
}

void LightSetup::update(float frame, uint32_t offset) const
{
	GraphicsInstance gfx = Graphics::getGraphics();

	auto colorValues = getColors(frame);
	gfx->updateConstantBuffer(offset, &colorValues.diffuse, sizeof(glm::vec3));
	gfx->updateConstantBuffer(offset + sizeof(glm::vec4), &colorValues.specular, sizeof(glm::vec3));

	auto rotation = getRotation(frame);
	auto direction = glm::rotate(rotation, { 0, 1, 0 });
	gfx->updateConstantBuffer(offset + 2 * sizeof(glm::vec4), &direction, sizeof(glm::vec3));
}

glm::quat LightSetup::getRotation(float frame) const
{
	if (m_rotations.getSize() < 2)
		return m_baseValues.rotation;

	// Binary search tree code structure copied from xutility
	size_t count = m_rotations.getSize();
	size_t index = 0;
	while (true) { // divide and conquer, find half that contains answer
		size_t count2 = (count / 2) & ~1;

		if (m_rotations[index + count2].time < frame) { // try top half
			const size_t amount = count2 + 2;
			index = amount;
			if (count <= amount)
				break;

			count -= amount;
		}
		else if (count2 == 0)
			break;
		count = count2;
	}

	if (index > 0)
		index -= 2;

	if (index >= m_rotations.getSize() || m_rotations[index].interpolation != InterpolationToggle::On)
		return m_rotations[index].object;

	return glm::mix(m_rotations[index].object, m_rotations[index + 2].object, (frame - m_rotations[index].time) * m_rotations[index + 1].coefficient);
}

LightColors LightSetup::getColors(float frame) const
{
	if (m_colors.isEmpty())
		return m_baseValues.colors;

	return InterpolateStruct(m_colors, frame);
}
