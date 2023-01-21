#pragma once
class XG_MaterialNode
{
public:
	virtual void bind(size_t slot) const = 0;
	virtual size_t getNumMaterials() const = 0;
};
