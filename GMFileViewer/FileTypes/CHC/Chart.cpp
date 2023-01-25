#include "Chart.h"

void Chart::load(FileReader& file)
{
	if (!file.checkTag("CHCH"))
		throw "Chart read error";

	file += 4;
	m_noteBuffer.reserve(file.read<uint32_t>());
	file += 4;

	if (file.read<uint32_t>() != 0)
		throw "Game crash";

	file += 16;
	file.read(m_pivotTime);
	file.read(m_endTime);

	m_noteBuffer.fill(file);

	const char* currPtr = m_noteBuffer.begin();
	parseNotesFromBuffer<Traceline>(currPtr);
	parseNotesFromBuffer<Phrase>(currPtr);
	parseNotesFromBuffer<Guard>(currPtr);
}

void Chart::save(FileWriter& file) const
{
	file.writeTag("CHCH");

	file << uint32_t(0x1300);
	m_noteBuffer.write_size(file);
	file << uint32_t(0) << uint32_t(0);

	static constexpr char ZERO[16]{};
	file << ZERO;

	file.write(m_pivotTime);
	file.write(m_endTime);

	m_noteBuffer.write_data(file);
}
