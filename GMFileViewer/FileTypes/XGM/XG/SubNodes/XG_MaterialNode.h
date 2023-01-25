#pragma once
#include "../XG.h"
class XG_MaterialNode : public XG_SubNode
{
public:
	using XG_SubNode::XG_SubNode;
	virtual void bind(size_t slot) const = 0;
	virtual size_t getNumMaterials() const = 0;
};
