#include "CHC.h"

CHC::CHC(const std::filesystem::path& filePath)
{
	FileReader file(filePath);
	if (!file.checkTag("SNGS"))
		throw "CHC file read error";

	file += 32;
	file.read(m_imcName);
	file.read(m_events);
	file.read(m_audio);
	file.read(m_speed);
	m_cues.reserve_and_fill(file);
	m_sections.construct(file);
	for (auto& section : m_sections)
		section.load(file);

	m_healthValues.reserve_and_fill(file);
}

void CHC::saveToFile(const std::filesystem::path& filePath) const
{
	FileWriter file(filePath);
	file.writeTag("SNGS");
	
	static constexpr char ZERO[12]{};
	file << uint32_t(0x1800) << ZERO << uint32_t(0x1069) << ZERO;
	file.write(m_imcName);
	file.write(m_events);
	file.write(m_audio);
	file.write(m_speed);
	m_cues.write_full(file);
	m_sections.write_size(file);
	for (const auto& section : m_sections)
		section.save(file);

	m_healthValues.write_full(file);
}
