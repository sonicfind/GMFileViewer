#pragma once
#include "SubNodes/XG_SubNode.h"

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

	std::vector<XG_SubNode::XGVectElement> m_nodes;
	std::vector<DagElement> m_dag;
	
public:
	void load(const char* input);

private:
	void fillDag(DagElement& dag, const char*& input);

	static std::unique_ptr<XG_SubNode> constructNode(std::string_view type);
};
