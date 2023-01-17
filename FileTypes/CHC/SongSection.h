#pragma once
#include <stdint.h>
#include "Chart.h"

struct Condition
{
	uint32_t type = 0;
	float argument = 0;
	int32_t trueEffect = 0;
	int32_t falseEffect = 0;
};

class SongSection
{
public:
	enum class Phase : uint32_t { INTRO, CHARGE, BATTLE, FINAL_AG, HARMONY, END, FINAL_I };

private:
	Phase m_phase = Phase::INTRO;
	float m_tempo = 120;
	uint32_t m_duration_samples = 0;
	GMArray<Condition> m_conditions;
	uint32_t m_numPlayers = 0;
	uint32_t m_numChartsPerPlayer = 0;
	GMArray<Chart> m_charts;

public:
	void load(FilePointer& file);
};
