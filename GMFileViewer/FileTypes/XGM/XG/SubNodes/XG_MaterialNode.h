#pragma once
class XG_MaterialNode
{
public:
	virtual void bind(uint32_t slot) const = 0;
	virtual size_t getNumMaterials() const = 0;
};
