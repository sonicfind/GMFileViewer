#pragma once
#include <stdint.h>

class Animation
{
	alignas(32) float m_length = 0;
	float m_keyframe_interval = 0;
	float m_framerate = 60;
	float m_starting_keyframe = 0;
	uint32_t m_non_tempo = 0;
};
