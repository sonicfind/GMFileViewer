#pragma once
#include "XG_SubNode.h"
class xgTexture : public XG_SubNode
{
	std::string m_url;
	unsigned long m_mipmap_depth = 0;
public:
	void load(const char*& input, const std::vector<XGVectElement>& nodes) override;
};

