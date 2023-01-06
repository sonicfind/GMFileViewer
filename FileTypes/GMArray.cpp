#include "GMArray.h"
#include "XGM/XG/SubNodes/VertexList.h"

template <>
void GMArrayLoader::loadElements(GMArray<VertexList>& arr, const char*& input)
{
	if (arr.reserve(input))
		for (auto& element : arr)
			element.load(input);
}