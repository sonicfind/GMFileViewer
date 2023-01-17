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
