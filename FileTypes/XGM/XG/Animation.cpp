#include "Animation.h"
#include "FileOperations.h"

Animation::Animation(const char* input)
{
	FileOps::Read(m_length, input);
	FileOps::Read(m_keyframe_interval, input);
	FileOps::Read(m_framerate, input);
	FileOps::Read(m_starting_keyframe, input);
	FileOps::Read(m_non_tempo, input);
}
