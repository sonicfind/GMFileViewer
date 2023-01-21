#pragma once
#include "VertexList.h"
class XG_UpdatableNode
{
public:
	virtual void updateVertices(VertexList& vertices) const = 0;
};
