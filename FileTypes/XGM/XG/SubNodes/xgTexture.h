#pragma once
#include "../XG.h"
#include <string>
class xgTexture : public XG_SubNode
{
	std::string m_url;
	unsigned long m_mipmap_depth = 0;
public:
	void load(FilePointer& file, const XG* xg) override;
};

