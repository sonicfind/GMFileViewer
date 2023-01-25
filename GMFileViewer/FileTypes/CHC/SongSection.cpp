#include "SongSection.h"
#include "FilePointer.h"

void SongSection::load(FilePointer& file)
{
	if (!file.checkTag("CHLS"))
		throw "SongSection read error";
	
	file += 32;
	file.read(m_phase);
	file.read(m_tempo);
	file.read(m_duration_samples);
	file += 4;

	m_conditions.reserve_and_fill(file);
	file.read(m_numPlayers);
	file.read(m_numChartsPerPlayer);
	m_charts.reserve(m_numPlayers * m_numChartsPerPlayer);
	for (auto& chart : m_charts)
		chart.load(file);
}

void SongSection::save(FileWriter& file) const
{
	file.writeTag("CHLS");

	static constexpr char ZERO[16]{};
	file << uint32_t(0x1300);
	file.write(ZERO, 12);
	file << ZERO;

	file.write(m_phase);
	file.write(m_tempo);
	file.write(m_duration_samples);
	file << uint32_t(0);

	m_conditions.write_full(file);
	file.write(m_numPlayers);
	file.write(m_numChartsPerPlayer);
	for (const auto& chart : m_charts)
		chart.save(file);
}
