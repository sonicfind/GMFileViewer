#include "XG.h"
#include "FileOperations.h"
#include "SubNodes/xgBgGeometry.h"
#include "SubNodes/xgBgMatrix.h"
#include "SubNodes/xgBone.h"
#include "SubNodes/xgDagMesh.h"
#include "SubNodes/xgDagTransform.h"
#include "SubNodes/xgEnvelope.h"
#include "SubNodes/xgMultiPassMaterial.h"
#include "SubNodes/XG_InterpolatorNodes.h"
#include "SubNodes/xgTexture.h"
#include "SubNodes/xgTime.h"

void XG::load(const char* input)
{
	if (!FileOps::checkTag("XGBv1.00", input))
		throw "XG file read error";

	std::string_view type = PString::Read(input);
	std::string_view name = PString::Read(input);
	while (PString::CheckForString_nothrow(";", input))
	{
		m_nodes.push_back({ std::string(name), constructNode(type) });

		PString::Read(type, input);
		PString::Read(name, input);
	}

	for (const auto& node : m_nodes)
	{
		PString::CheckForString("{", input);
		node.second->load(input, this);
		PString::CheckForString("}", input);

		if (PString::CheckForString_nothrow("dag", input))
			break;

		PString::Read(type, input);
		PString::Read(name, input);
	}

	PString::CheckForString("{", input);
	while (!PString::CheckForString_nothrow("}", input))
	{
		DagElement& dag = m_dag.emplace_back(searchForNode(input));
		PString::CheckForString("[", input);
		fillDag(dag, input);
	}
}

std::unique_ptr<XG_SubNode> XG::constructNode(std::string_view type)
{
	if (type == "xgVec3Interpolator")			return std::make_unique<xgVec3Interpolator>();
	else if (type == "xgQuatInterpolator") 		return std::make_unique<xgQuatInterpolator>();
	else if (type == "xgBone") 					return std::make_unique<xgBone>();
	else if (type == "xgBgMatrix")				return std::make_unique<xgBgMatrix>();
	else if (type == "xgEnvelope") 				return std::make_unique<xgEnvelope>();
	else if (type == "xgMaterial") 				return std::make_unique<xgMaterial>();
	else if (type == "xgTexture") 				return std::make_unique<xgTexture>();
	else if (type == "xgDagMesh") 				return std::make_unique<xgDagMesh>();
	else if (type == "xgBgGeometry") 			return std::make_unique<xgBgGeometry>();
	else if (type == "xgDagTransform") 			return std::make_unique<xgDagTransform>();
	else if (type == "xgMultiPassMaterial") 	return std::make_unique<xgMultiPassMaterial>();
	else if (type == "xgVertexInterpolator") 	return std::make_unique<xgVertexInterpolator>();
	else if (type == "xgNormalInterpolator") 	return std::make_unique<xgNormalInterpolator>();
	else if (type == "xgShapeInterpolator") 	return std::make_unique<xgShapeInterpolator>();
	else if (type == "xgTexCoordInterpolator")	return std::make_unique<xgTexCoordInterpolator>();
	else if (type == "xgTime")					return std::make_unique<xgTime>();
	else
		throw "Unrecognized node";
}

XG_SubNode* XG::searchForNode(const char*& input) const
{
	return searchForNode(PString::Read(input));
}

XG_SubNode* XG::searchForNode(std::string_view name) const
{
	for (const auto& node : m_nodes)
		if (node.first == name)
			return node.second.get();
	return nullptr;
}

XG_SubNode* XG::grabNode_optional(std::string_view inputString, std::string_view outputString, const char*& input) const
{
	if (!PString::CheckForString_nothrow(inputString, input))
		return nullptr;

	XG_SubNode* node = searchForNode(input);
	if (node == nullptr)
		throw "Node not located";

	if (!PString::CheckForString_nothrow(outputString, input))
		throw "Output string not matched";

	return node;
}

XG_SubNode* XG::grabNode(std::string_view inputString, std::string_view outputString, const char*& input) const
{
	XG_SubNode* node = grabNode_optional(inputString, outputString, input);
	if (node == nullptr)
		throw "Input string not matched";
	return node;
}

void XG::fillDag(DagElement& dag, const char*& input)
{
	while (!PString::CheckForString_nothrow("]", input))
	{
		DagElement& newDag = dag.connections.emplace_back(searchForNode(input));
		if (PString::CheckForString_nothrow("[", input))
			fillDag(newDag, input);
	}
}
