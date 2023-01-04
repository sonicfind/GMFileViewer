#pragma once
#include <vector>
#include <memory>
#include "../PString.h"

class XG_SubNode
{
public:
	using XGVectElement = std::pair<std::string, std::unique_ptr<XG_SubNode>>;

	template <typename T = XG_SubNode>
	static T* searchForNode(const char*& input, const std::vector<XGVectElement>& nodes)
	{
		const std::string name = PString::Read(input);
		for (const auto& node : nodes)
			if (node.first == name)
				return static_cast<T*>(node.second.get());
		return nullptr;
	}

protected:
	template <typename T>
	static void BindNodes(std::vector<T*>& nodeList, std::string_view inputString, std::string_view outputString, const char*& input, const std::vector<XGVectElement>& nodes)
	{
		while (PString::CheckForString_nothrow(inputString, input))
		{
			nodeList.push_back(searchForNode<T>(input, nodes));
			PString::CheckForString(outputString, input);
		}
	}

	template <typename T>
	static bool BindNode_optional(T*& node, std::string_view inputString, std::string_view outputString, const char*& input, const std::vector<XGVectElement>& nodes)
	{
		if (!PString::CheckForString_nothrow(inputString, input))
			return false;

		node = searchForNode<T>(input, nodes);

		if (!PString::CheckForString_nothrow(outputString, input))
			throw "Output string not matched";

		return true;
	}

	template <typename T>
	static void BindNode(T*& node, std::string_view inputString, std::string_view outputString, const char*& input, const std::vector<XGVectElement>& nodes)
	{
		if (!BindNode_optional(node, inputString, outputString, input, nodes))
			throw "Input string not matched";
	}

public:
	virtual void load(const char*& input, const std::vector<XGVectElement>& nodes) = 0;
	virtual ~XG_SubNode() {}
};
