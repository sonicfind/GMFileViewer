#include "xgTime.h"
#include "../PString.h"
#include "FileOperations.h"

void xgTime::load(const char*& input, const XG* xg)
{
	PString::ReadNamedValue("numFrames", m_numFrames, input);
	PString::ReadNamedValue("time", m_time, input);
}

void xgTime::setTime(float time)
{
	m_time = time;
}

float xgTime::getTime() const noexcept
{
	return m_time;
}
