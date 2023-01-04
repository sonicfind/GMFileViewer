#pragma once
#include "FileOperations.h"

template <typename T>
struct XGArray
{
	uint32_t size = 0;
	std::unique_ptr<T[]> elements;
};

class VertexList;
namespace XGArrayLoader
{
	template <typename T>
	void loadElements(XGArray<T>& arr, const char*& input)
	{
		FileOps::Read(arr.size, input);
		if (arr.size > 0)
			FileOps::Read_Array_Count(arr.elements, arr.size, input);
	}

	template <class T>
	void loadElements(XGArray<XGArray<T>>& arr, const char*& input)
	{
		FileOps::Read(arr.size, input);
		if (arr.size > 0)
		{
			arr.elements = std::make_unique<XGArray<T>[]>(arr.size);
			for (uint32_t i = 0; i < arr.size; ++i)
				loadElements(arr.elements[i], input);
		}
	}

	template <>
	void loadElements(XGArray<VertexList>& arr, const char*& input);
}
