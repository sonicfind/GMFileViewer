#include "xgTime.h"
#include "../PString.h"
#include "FilePointer.h"

void xgTime::load(FilePointer& file, const XG* xg)
{
	PString::ReadNamedValue("numFrames", m_numFrames, file);
	PString::ReadNamedValue("time", m_time, file);
}

void xgTime::setTime(float time)
{
	m_time = time;
}

float xgTime::getTime() const noexcept
{
	return m_time;
}
