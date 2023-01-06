#include "Chart.h"

void Chart::load(const char*& input)
{
	if (!FileOps::checkTag("CHCH", input))
		throw "Chart read error";

	input += 4;
	m_noteBuffer.reserve(FileOps::Read<uint32_t>(input));
	input += 4;

	if (FileOps::Read<uint32_t>(input) != 0)
		throw "Game crash";

	input += 16;
	FileOps::Read(m_pivotTime, input);
	FileOps::Read(m_endTime, input);

	m_noteBuffer.fill(input);

	const char* currPtr = m_noteBuffer.begin();
	addNotesFromFile<Traceline>(currPtr);
	addNotesFromFile<Phrase>(currPtr);
	addNotesFromFile<Guard>(currPtr);
}
