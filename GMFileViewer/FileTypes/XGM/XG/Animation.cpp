#include "Animation.h"
#include "FileReader.h"

float Animation::calcLength(float tempo) const
{
	if (m_non_tempo)
		return m_length;
	else
		return (225 * m_length) / tempo;
}

float Animation::getTimelinePosition(float frame, float tempo, PlaybackDirection playbackDirection) const
{
	if (!m_non_tempo)
		frame *= tempo / 225;

	if (frame >= m_length)
		frame = m_length;

	if (playbackDirection == PlaybackDirection::BACKWARDS)
		frame = m_length - frame;

	return frame / m_keyframe_interval + m_starting_keyframe;
}
