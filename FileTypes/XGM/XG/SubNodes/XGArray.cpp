#include "XGArray.h"
#include "VertexList.h"

template <>
void XGArrayLoader::loadElements(XGArray<VertexList>& arr, const char*& input)
{
	FileOps::Read(arr.size, input);
	if (arr.size > 0)
	{
		arr.elements = std::make_unique<VertexList[]>(arr.size);
		for (uint32_t i = 0; i < arr.size; ++i)
			arr.elements[i].load(input);
	}
}