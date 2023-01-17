#include "XG.h"
#include "FilePointer.h"
#include "SubNodes/xgBgGeometry.h"
#include "SubNodes/xgBgMatrix.h"
#include "SubNodes/xgBone.h"
#include "SubNodes/xgDagMesh.h"
#include "SubNodes/xgDagTransform.h"
#include "SubNodes/xgEnvelope.h"
#include "SubNodes/xgMultiPassMaterial.h"
#include "SubNodes/xgNormalInterpolator.h"
#include "SubNodes/xgQuatInterpolator.h"
#include "SubNodes/xgShapeInterpolator.h"
#include "SubNodes/xgTexCoordInterpolator.h"
#include "SubNodes/xgTexture.h"
#include "SubNodes/xgTime.h"
#include "SubNodes/xgVec3Interpolator.h"
#include "SubNodes/xgVertexInterpolator.h"

void XG::load(FilePointer file)
{
	if (!file.checkTag("XGBv1.00"))
		throw "XG file read error";

	std::string_view type = PString::Read(file);
	std::string_view name = PString::Read(file);
	while (PString::CheckForString_nothrow(";", file))
	{
		m_nodes.push_back({ std::string(name), constructNode(type) });

		PString::Read(type, file);
		PString::Read(name, file);
	}

	for (const auto& node : m_nodes)
	{
		PString::CheckForString("{", file);
		node.second->load(file, this);
		PString::CheckForString("}", file);

		if (PString::CheckForString_nothrow("dag", file))
			break;

		PString::Read(type, file);
		PString::Read(name, file);
	}

	PString::CheckForString("{", file);
	while (!PString::CheckForString_nothrow("}", file))
	{
		DagElement& dag = m_dag.emplace_back(searchForNode(file));
		PString::CheckForString("[", file);
		fillDag(dag, file);
	}

	if (auto time = static_cast<xgTime*>(searchForNode("time")))
		m_time = time;
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

XG_SubNode* XG::searchForNode(FilePointer& file) const
{
	return searchForNode(PString::Read(file));
}

XG_SubNode* XG::searchForNode(std::string_view name) const
{
	for (const auto& node : m_nodes)
		if (node.first == name)
			return node.second.get();
	return nullptr;
}

XG_SubNode* XG::grabNode_optional(std::string_view inputString, std::string_view outputString, FilePointer& file) const
{
	if (!PString::CheckForString_nothrow(inputString, file))
		return nullptr;

	XG_SubNode* node = searchForNode(file);
	if (node == nullptr)
		throw "Node not located";

	if (!PString::CheckForString_nothrow(outputString, file))
		throw "Output string not matched";

	return node;
}

XG_SubNode* XG::grabNode(std::string_view inputString, std::string_view outputString, FilePointer& file) const
{
	XG_SubNode* node = grabNode_optional(inputString, outputString, file);
	if (node == nullptr)
		throw "Input string not matched";
	return node;
}

void XG::fillDag(DagElement& dag, FilePointer& file)
{
	while (!PString::CheckForString_nothrow("]", file))
	{
		DagElement& newDag = dag.connections.emplace_back(searchForNode(file));
		if (PString::CheckForString_nothrow("[", file))
			fillDag(newDag, file);
	}
}
