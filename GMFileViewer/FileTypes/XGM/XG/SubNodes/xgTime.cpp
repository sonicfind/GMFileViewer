#include "xgTime.h"
#include "../PString.h"
#include "FileReader.h"

void xgTime::load(FileReader& file, const XG* xg)
{
	PString::ReadNamedValue("numFrames", m_numFrames, file);
	PString::ReadNamedValue("time", m_time, file);
}

void xgTime::writeType(FileWriter& file) const
{
	PString::WriteString("xgTime", file);
}

void xgTime::save(FileWriter& file) const
{
	PString::WriteNamedValue("numFrames", m_numFrames, file);
	PString::WriteNamedValue("time", m_time, file);
}

void xgTime::setTime(float time)
{
	m_time = time;
}

float xgTime::getTime() const noexcept
{
	return m_time;
}
