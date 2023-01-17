#include "Chart.h"

void Chart::load(FilePointer& file)
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
	addNotesFromFile<Traceline>(currPtr);
	addNotesFromFile<Phrase>(currPtr);
	addNotesFromFile<Guard>(currPtr);
}
