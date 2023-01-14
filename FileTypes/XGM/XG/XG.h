#pragma once
#include "PString.h"

class XG;
class XG_SubNode
{
public:
	virtual void load(const char*& input, const XG* xg) = 0;
	virtual ~XG_SubNode() {}
};

class xgTime;
class XG
{
	enum class SubNodeType
	{
		Vec3Interpolator,
		QuatInterpolator,
		Bone,
		BgMatrix,
		Envelope,
		Material,
		Texture,
		DagMesh,
		BgGeometry,
		DagTransform,
		MultiPassMaterial,
		VertexInterpolator,
		NormalInterpolator,
		ShapeInterpolator,
		TexCoordInterpolator,
		Time,
	};

	struct DagElement
	{
		XG_SubNode* base;
		std::vector<DagElement> connections;

		DagElement(XG_SubNode* node) : base(node) {}
	};

	std::vector<std::pair<std::string, std::unique_ptr<XG_SubNode>>> m_nodes;
	std::vector<DagElement> m_dag;
	xgTime* m_time = nullptr;
	
public:
	void load(const char* input);

private:
	void fillDag(DagElement& dag, const char*& input);

	static std::unique_ptr<XG_SubNode> constructNode(std::string_view type);

public:
	XG_SubNode* searchForNode(const char*& input) const;
	XG_SubNode* searchForNode(std::string_view name) const;
	XG_SubNode* grabNode_optional(std::string_view inputString, std::string_view outputString, const char*& input) const;
	XG_SubNode* grabNode(std::string_view inputString, std::string_view outputString, const char*& input) const;

	template <typename NodeType = XG_SubNode, typename = std::enable_if<std::is_base_of<XG_SubNode, NodeType>::value>>
	bool grabNode_nondestructive(NodeType*& dst, std::string_view inputString, std::string_view outputString, const char*& input) const
	{
		XG_SubNode* node = grabNode_optional(inputString, outputString, input);
		if (node == nullptr)
			return false;

		dst = static_cast<NodeType*>(node);
		return true;
	}
};
