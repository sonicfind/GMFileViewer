#pragma once
#include "PString.h"

class XG;
class XG_SubNode
{
public:
	virtual void load(FilePointer& file, const XG* xg) = 0;
	virtual ~XG_SubNode() {}
};

class xgTime;
class xgDagMesh;
class xgDagTransform;
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
		xgDagMesh* m_mesh;
		xgDagTransform* m_transform;
		std::vector<DagElement> m_connections;

		DagElement(XG_SubNode* node);
		void update() const;
	};

	std::vector<std::pair<std::string, std::unique_ptr<XG_SubNode>>> m_nodes;
	std::vector<DagElement> m_dag;
	xgTime* m_time = nullptr;
	
public:
	void load(FilePointer file);
	void update(float frame) const;

private:
	void fillDag(DagElement& dag, FilePointer& file);

	static std::unique_ptr<XG_SubNode> constructNode(std::string_view type);

public:
	XG_SubNode* searchForNode(FilePointer& file) const;
	XG_SubNode* searchForNode(std::string_view name) const;
	XG_SubNode* grabNode_optional(std::string_view inputString, std::string_view outputString, FilePointer& file) const;
	XG_SubNode* grabNode(std::string_view inputString, std::string_view outputString, FilePointer& file) const;

	template <typename NodeType = XG_SubNode, typename = std::enable_if<std::is_base_of<XG_SubNode, NodeType>::value>>
	bool grabNode_nondestructive(NodeType*& dst, std::string_view inputString, std::string_view outputString, FilePointer& file) const
	{
		XG_SubNode* node = grabNode_optional(inputString, outputString, file);
		if (node == nullptr)
			return false;

		dst = static_cast<NodeType*>(node);
		return true;
	}
};
