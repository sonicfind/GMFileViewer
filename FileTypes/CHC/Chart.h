#pragma once
#include "GMArray.h"
#include "Notes/Traceline.h"
#include "Notes/Phrase.h"
#include "Notes/Guard.h"

class Chart
{
	struct NotePair
	{
		const Note* m_note;
		enum NoteType
		{
			NOTE,
			TRACELINE,
			PHRASE,
			GUARD
		} m_type;

		NotePair(const Traceline* traceline) : m_note(traceline), m_type(TRACELINE) {}
		NotePair(const Phrase* phrase) : m_note(phrase), m_type(PHRASE) {}
		NotePair(const Guard* guard) : m_note(guard), m_type(GUARD) {}
		int32_t getPivotAlpha() const { return m_note->m_pivotAlpha; }
	};

	GMArray<const char> m_noteBuffer;
	int32_t m_pivotTime = 0;
	int32_t m_endTime = 0;
	std::vector<NotePair> m_timeline;

public:
	void load(const char*& input);

private:
	template<typename T, typename = std::enable_if<std::is_base_of<Note, T>::value>>
	void addNotesFromFile(const char*& bufferPtr)
	{
		if (bufferPtr + sizeof(uint32_t) > m_noteBuffer.end())
			throw "dumbass chart file";

		const uint32_t numNotes = FileOps::Read<uint32_t>(bufferPtr);
		if (bufferPtr + numNotes * sizeof(T) > m_noteBuffer.end())
			throw "dumbass chart file";

		m_timeline.reserve(m_timeline.size() + numNotes);
		auto iter = m_timeline.begin();

		const T* notePtr = reinterpret_cast<const T*>(bufferPtr);
		for (uint32_t i = 0; i < numNotes; ++i, ++notePtr, ++iter)
		{
			while (iter != m_timeline.end() && iter->getPivotAlpha() <= notePtr->m_pivotAlpha)
				++iter;

			iter = m_timeline.insert(iter, notePtr);
		}

		bufferPtr = reinterpret_cast<const char*>(notePtr);
	}
};
