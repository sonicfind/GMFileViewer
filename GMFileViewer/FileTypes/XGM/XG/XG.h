#pragma once
#include "PString.h"
#include <glm/glm.hpp>

class XG;
class XG_SubNode
{
protected:
	std::string m_name;

public:
	XG_SubNode(std::string_view name);
	void writeName(FileWriter& file) const;
	std::string_view getName() const { return m_name; }

	virtual void load(FileReader& file, const XG* xg) = 0;
	virtual void writeType(FileWriter& file) const = 0;
	virtual void save(FileWriter& file) const = 0;
	virtual ~XG_SubNode() {}

	static void WriteNode(std::string_view inputString, std::string_view outputString, const XG_SubNode* const node, FileWriter& file);
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
		void draw(glm::mat4 meshMatrix) const;
	};

	std::vector<std::unique_ptr<XG_SubNode>> m_nodes;
	std::vector<DagElement> m_dag;
	xgTime* m_time = nullptr;
	
public:
	void load(FileReader file);
	void save(FileWriter& file) const;
	void createVertexBuffers();
	void update(float frame) const;
	void draw(const glm::mat4& modelMatrix) const;

private:
	void fillDag(DagElement& dag, FileReader& file);
	void saveDag(const DagElement& dag, FileWriter& file, bool forceBrackets) const;

	static std::unique_ptr<XG_SubNode> constructNode(std::string_view type, std::string_view name);

public:
	XG_SubNode* searchForNode(FileReader& file) const;
	XG_SubNode* searchForNode(std::string_view name) const;
	XG_SubNode* grabNode_optional(std::string_view inputString, std::string_view outputString, FileReader& file) const;
	XG_SubNode* grabNode(std::string_view inputString, std::string_view outputString, FileReader& file) const;

	template <typename NodeType = XG_SubNode, typename = std::enable_if<std::is_base_of<XG_SubNode, NodeType>::value>>
	bool grabNode_nondestructive(NodeType*& dst, std::string_view inputString, std::string_view outputString, FileReader& file) const
	{
		XG_SubNode* node = grabNode_optional(inputString, outputString, file);
		if (node == nullptr)
			return false;

		if (NodeType* convert = dynamic_cast<NodeType*>(node))
			dst = convert;
		return true;
	}
};
