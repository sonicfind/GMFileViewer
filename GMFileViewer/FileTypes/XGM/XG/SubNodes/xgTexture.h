#pragma once
#include "../XG.h"
#include <string>
class xgTexture : public XG_SubNode
{
	char m_url[16];
	unsigned long m_mipmap_depth = 0;

public:
	using XG_SubNode::XG_SubNode;
	void load(FileReader& file, const XG* xg) override;

	void writeType(FileWriter& file) const override;
	void save(FileWriter& file) const override;

	void bind() const;
};

