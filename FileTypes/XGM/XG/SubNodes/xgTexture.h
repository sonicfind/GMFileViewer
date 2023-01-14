#pragma once
#include "../XG.h"
#include <string>
class xgTexture : public XG_SubNode
{
	std::string m_url;
	unsigned long m_mipmap_depth = 0;
public:
	void load(const char*& input, const XG* xg) override;
};

