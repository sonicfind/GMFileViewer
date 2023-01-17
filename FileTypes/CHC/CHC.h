#pragma once
#include "SongSection.h"

struct SSQRange
{
	float first = 0;
	float last = 0;
};

struct AudioChannel
{
	uint32_t volume = 32767;
	uint32_t pan = 16383;
};

struct Cue
{
	alignas(16) uint32_t index;
	char name[16];
	char audio[16];
	SSQRange frames;
};

struct HealthValues
{
	float initialEnergy = .2f;
	float chargeInitial = .01f;
	float attackInitial = .015f;
	float guardEnergy = .015f;
	float attackMiss = .05f;
	float guardMiss = .05f;
	float chargeRelease = .025f;
	float attackRelease = .025f;
};

class CHC
{
	char m_imcName[256];
	SSQRange m_events[4];
	AudioChannel m_audio[8];
	float m_speed;
	GMArray<Cue> m_cues;
	GMArray<SongSection> m_sections;
	GMArray<HealthValues> m_healthValues;

public:
	CHC(const std::filesystem::path& filePath);
};
