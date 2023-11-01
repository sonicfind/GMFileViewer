#include "xgTexture.h"
#include "../PString.h"
#include "FileReader.h"
#include "Graphics.h"

void xgTexture::load(FileReader& file, const XG* xg)
{
	PString::ThrowOnStringMismatch("url", file);
	std::string_view parsed = PString::GetString(file);
	for (size_t i = 0; i < parsed.size(); ++i)
		m_url[i] = std::toupper(parsed[i]);

	PString::ReadNamedValue("mipmap_depth", m_mipmap_depth, file);
}

void xgTexture::writeType(FileWriter& file) const
{
	PString::WriteString("xgTexture", file);
}

void xgTexture::save(FileWriter& file) const
{
	char buffer[16]{};
	for (size_t i = 0; i < 16 && m_url[i]; ++i)
		buffer[i] = std::tolower(m_url[i]);

	PString::WriteString("url", file);
	PString::WriteString(buffer, file);

	PString::WriteNamedValue("mipmap_depth", m_mipmap_depth, file);
}

void xgTexture::bind(Graphics& gfx) const
{
	gfx.bindTexture(m_url);
}
