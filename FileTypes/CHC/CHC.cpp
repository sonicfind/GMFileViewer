#include "CHC.h"

CHC::CHC(const std::filesystem::path& filePath)
{
	const FileOps::FilePointers file(filePath);
	auto input = file.begin();

	if (!FileOps::checkTag("SNGS", input))
		throw "CHC file read error";

	input += 32;
	FileOps::Read(m_imcName, input);
	FileOps::Read(m_events, input);
	FileOps::Read(m_audio, input);
	FileOps::Read(m_speed, input);
	m_cues.reserve_and_fill(input);
	m_sections.reserve(input);
	for (auto& section : m_sections)
		section.load(input);

	m_healthValues.reserve_and_fill(input);
}
