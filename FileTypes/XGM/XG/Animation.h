#pragma once

class Animation
{
	float m_length = 0;
	float m_keyframe_interval = 0;
	float m_framerate = 60;
	float m_starting_keyframe = 0;
	unsigned long m_non_tempo = 0;

public:
	Animation(const char* input);
};
