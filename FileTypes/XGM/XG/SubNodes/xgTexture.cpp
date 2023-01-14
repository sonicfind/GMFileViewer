#include "xgTexture.h"
#include "../PString.h"
#include "FileOperations.h"

void xgTexture::load(const char*& input, const XG* xg)
{
	PString::ReadNamedValue("url", m_url, input);
	PString::ReadNamedValue("mipmap_depth", m_mipmap_depth, input);
}
