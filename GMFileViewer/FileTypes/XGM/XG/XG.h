#pragma once
#include "PString.h"
#include <DirectXMath.h>

class XG;
class XG_SubNode
{
public:
	virtual void load(FilePointer& file, const XG* xg) = 0;
	virtual void writeType(FileWriter& file) const = 0;
	virtual void save(FileWriter& file, const XG* xg) const = 0;
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
		xgDagMesh* m_mesh = nullptr;
		xgDagTransform* m_transform = nullptr;
		std::vector<DagElement> m_connections;

		DagElement(XG_SubNode* node);

		void createVertexBuffers();
		void update() const;
		void draw(DirectX::XMMATRIX meshMatrix) const;
	};

	std::vector<std::pair<std::string, std::unique_ptr<XG_SubNode>>> m_nodes;
	std::vector<DagElement> m_dag;
	xgTime* m_time = nullptr;
	
public:
	void load(FilePointer file);
	void save(FileWriter& file) const;
	void createVertexBuffers();
	void update(float frame) const;
	void draw(const DirectX::XMMATRIX& modelMatrix) const;

private:
	void fillDag(DagElement& dag, FilePointer& file);
	template <bool forceBrackets>
	void saveDag(const DagElement& dag, FileWriter& file) const
	{
		PString::WriteString(getNodeName(dag.m_mesh ? (XG_SubNode*)dag.m_mesh : dag.m_transform), file);
		if (!dag.m_connections.empty())
		{
			PString::WriteString("[", file);
			for (const auto& connection : dag.m_connections)
				saveDag<false>(connection, file);
			PString::WriteString("]", file);
		}
		else if constexpr (forceBrackets)
		{
			PString::WriteString("[", file);
			PString::WriteString("]", file);
		}

	}

	static std::unique_ptr<XG_SubNode> constructNode(std::string_view type);

public:
	XG_SubNode* searchForNode(FilePointer& file) const;
	XG_SubNode* searchForNode(std::string_view name) const;
	std::string_view getNodeName(const XG_SubNode* nodeToFind) const;

	XG_SubNode* grabNode_optional(std::string_view inputString, std::string_view outputString, FilePointer& file) const;
	XG_SubNode* grabNode(std::string_view inputString, std::string_view outputString, FilePointer& file) const;
	void writeNode(std::string_view inputString, std::string_view outputString, const XG_SubNode* const node, FileWriter& file) const;

	template <typename NodeType = XG_SubNode, typename = std::enable_if<std::is_base_of<XG_SubNode, NodeType>::value>>
	bool grabNode_nondestructive(NodeType*& dst, std::string_view inputString, std::string_view outputString, FilePointer& file) const
	{
		XG_SubNode* node = grabNode_optional(inputString, outputString, file);
		if (node == nullptr)
			return false;

		if (NodeType* convert = dynamic_cast<NodeType*>(node))
			dst = convert;
		return true;
	}
};
