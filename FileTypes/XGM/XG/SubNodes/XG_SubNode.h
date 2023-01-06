#pragma once
#include <vector>
#include <memory>
#include "../PString.h"

class XG_SubNode
{
public:
	using XGVectElement = std::pair<std::string, std::unique_ptr<XG_SubNode>>;

	template <typename NodeType = XG_SubNode, typename = std::enable_if<std::is_base_of<XG_SubNode, NodeType>::value>>
	static NodeType* searchForNode(const char*& input, const std::vector<XGVectElement>& nodes)
	{
		std::string_view name = PString::Read(input);
		for (const auto& node : nodes)
			if (node.first == name)
				return static_cast<NodeType*>(node.second.get());
		return nullptr;
	}

protected:
	template <typename NodeType = XG_SubNode, typename = std::enable_if<std::is_base_of<XG_SubNode, NodeType>::value>>
	static void BindNodes(std::vector<NodeType*>& nodeList, std::string_view inputString, std::string_view outputString, const char*& input, const std::vector<XGVectElement>& nodes)
	{
		while (PString::CheckForString_nothrow(inputString, input))
		{
			nodeList.push_back(searchForNode<NodeType>(input, nodes));
			PString::CheckForString(outputString, input);
		}
	}

	template <typename NodeType = XG_SubNode, typename = std::enable_if<std::is_base_of<XG_SubNode, NodeType>::value>>
	static bool BindNode_optional(NodeType*& node, std::string_view inputString, std::string_view outputString, const char*& input, const std::vector<XGVectElement>& nodes)
	{
		if (!PString::CheckForString_nothrow(inputString, input))
			return false;

		node = searchForNode<NodeType>(input, nodes);

		if (!PString::CheckForString_nothrow(outputString, input))
			throw "Output string not matched";

		return true;
	}

	template <typename NodeType = XG_SubNode, typename = std::enable_if<std::is_base_of<XG_SubNode, NodeType>::value>>
	static void BindNode(NodeType*& node, std::string_view inputString, std::string_view outputString, const char*& input, const std::vector<XGVectElement>& nodes)
	{
		if (!BindNode_optional(node, inputString, outputString, input, nodes))
			throw "Input string not matched";
	}

public:
	virtual void load(const char*& input, const std::vector<XGVectElement>& nodes) = 0;
	virtual ~XG_SubNode() {}
};
