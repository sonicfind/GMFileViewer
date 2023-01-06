#include "SongSection.h"
#include "FileOperations.h"

void SongSection::load(const char*& input)
{
	if (!FileOps::checkTag("CHLS", input))
		throw "SongSection read error";
	
	input += 32;
	FileOps::Read(m_phase, input);
	FileOps::Read(m_tempo, input);
	FileOps::Read(m_duration_samples, input);
	input += 4;

	m_conditions.reserve_and_fill(input);
	FileOps::Read(m_numPlayers, input);
	FileOps::Read(m_numChartsPerPlayer, input);
	m_charts.reserve(m_numPlayers * m_numChartsPerPlayer);
	for (auto& chart : m_charts)
		chart.load(input);
}
