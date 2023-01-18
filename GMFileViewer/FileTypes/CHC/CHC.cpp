#include "CHC.h"

CHC::CHC(const std::filesystem::path& filePath)
{
	FilePointer file(filePath);
	if (!file.checkTag("SNGS"))
		throw "CHC file read error";

	file += 32;
	file.read(m_imcName);
	file.read(m_events);
	file.read(m_audio);
	file.read(m_speed);
	m_cues.reserve_and_fill(file);
	m_sections.reserve(file);
	for (auto& section : m_sections)
		section.load(file);

	m_healthValues.reserve_and_fill(file);
}
