#include "xgTexture.h"
#include "../PString.h"
#include "FilePointer.h"
#include "Graphics.h"

void xgTexture::load(FilePointer& file, const XG* xg)
{
	PString::ReadNamedValue("url", m_url, file);
	PString::ReadNamedValue("mipmap_depth", m_mipmap_depth, file);
}

void xgTexture::bind() const
{
	Graphics::getGraphics()->bindTexture(m_url);
}
