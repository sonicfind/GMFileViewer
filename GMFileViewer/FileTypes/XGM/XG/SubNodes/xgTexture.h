#pragma once
#include "../XG.h"
#include <string>
class xgTexture : public XG_SubNode
{
	char m_url[16];
	unsigned long m_mipmap_depth = 0;
public:
	void load(FilePointer& file, const XG* xg) override;
	void bind() const;
};

