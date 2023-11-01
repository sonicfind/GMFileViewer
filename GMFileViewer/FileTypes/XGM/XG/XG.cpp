#include "XG.h"
#include "FileReader.h"
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

XG_SubNode::XG_SubNode(std::string_view name) : m_name(name) {}

void XG_SubNode::writeName(FileWriter& file) const
{
	PString::WriteString(m_name, file);
}

void XG_SubNode::WriteNode(std::string_view inputString, std::string_view outputString, const XG_SubNode* const node, FileWriter& file)
{
	PString::WriteString(inputString, file);
	node->writeName(file);
	PString::WriteString(outputString, file);
}

void XG::load(FileReader& file)
{
	if (!file.checkTag("XGBv1.00"))
		throw "XG file read error";

	std::string_view type = PString::GetString(file);
	std::string_view name = PString::GetString(file);
	while (PString::CheckForString(";", file))
	{
		m_nodes.push_back(constructNode(type, name));
		type = PString::GetString(file);
		name = PString::GetString(file);
	}

	for (const auto& node : m_nodes)
	{
		PString::ThrowOnStringMismatch("{", file);
		node->load(file, this);
		PString::ThrowOnStringMismatch("}", file);

		if (PString::CheckForString("dag", file))
			break;

		type = PString::GetString(file);
		name = PString::GetString(file);
	}

	PString::ThrowOnStringMismatch("{", file);
	while (!PString::CheckForString("}", file))
	{
		DagElement& dag = m_dag.emplace_back(searchForNode(file));
		PString::ThrowOnStringMismatch("[", file);
		fillDag(dag, file);
	}

	if (auto time = static_cast<xgTime*>(searchForNode("time")))
		m_time = time;
}

void XG::save(FileWriter& file) const
{
	file.writeTag("XGBv1.00");
	for (const auto& node : m_nodes)
	{
		node->writeType(file);
		node->writeName(file);
		PString::WriteString(";", file);
	}

	for (const auto& node : m_nodes)
	{
		node->writeType(file);
		node->writeName(file);
		PString::WriteString("{", file);
		node->save(file);
		PString::WriteString("}", file);
	}

	PString::WriteString("dag", file);
	PString::WriteString("{", file);
	for (const auto& dag : m_dag)
		saveDag(dag, file, true);
	PString::WriteString("}", file);
}

void XG::createVertexBuffers(Graphics& gfx)
{
	for (auto& dag : m_dag)
		dag.createVertexBuffers(gfx);
}

void XG::addInstance(Graphics& gfx)
{
	for (auto& dag : m_dag)
		dag.addInstance(gfx);
}

void XG::update(Graphics& gfx, uint32_t instance, float frame, const glm::mat4& modelMatrix)
{
	if (m_time)
		m_time->setTime(frame);

	for (auto& dag : m_dag)
		dag.update(gfx, instance, modelMatrix);
}

void XG::draw(Graphics& gfx, uint32_t instance, bool doTransparentMeshes) const
{
	for (const auto& dag : m_dag)
		dag.draw(gfx, instance, doTransparentMeshes);
}

XG::DagElement::DagElement(XG_SubNode* node)
{
	if (!(m_mesh = dynamic_cast<xgDagMesh*>(node)) && !(m_transform = dynamic_cast<xgDagTransform*>(node)))
		throw std::runtime_error("Invalid node in dag map");
}

void XG::DagElement::createVertexBuffers(Graphics& gfx)
{
	if (m_mesh)
		m_mesh->createVertexBuffer(gfx);

	m_meshMatrices.clear();
	m_meshMatrices.emplace_back();

	for (auto& dag : m_connections)
		dag.createVertexBuffers(gfx);
}

void XG::DagElement::addInstance(Graphics& gfx)
{
	if (m_mesh)
		m_mesh->addInstance(gfx);

	m_meshMatrices.emplace_back();

	for (auto& dag : m_connections)
		dag.addInstance(gfx);
}

void XG::DagElement::update(Graphics& gfx, uint32_t instance, glm::mat4 meshMatrix)
{
	m_meshMatrices[instance] = meshMatrix;

	if (m_mesh)
		m_mesh->update(gfx, instance);
	else if (m_transform)
		m_meshMatrices[instance] *= m_transform->calcTransformMatrix();

	for (auto& dag : m_connections)
		dag.update(gfx, instance, m_meshMatrices[instance]);
}

void XG::DagElement::draw(Graphics& gfx, uint32_t instance, bool doTransparentMeshes) const
{
	if (m_mesh && m_mesh->hasTransparency() == doTransparentMeshes)
		m_mesh->draw(gfx, instance, m_meshMatrices[instance]);

	for (const auto& dag : m_connections)
		dag.draw(gfx, instance, doTransparentMeshes);
}

std::unique_ptr<XG_SubNode> XG::constructNode(std::string_view type, std::string_view name)
{
	if (type == "xgVec3Interpolator")			return std::make_unique<xgVec3Interpolator>(name);
	else if (type == "xgQuatInterpolator") 		return std::make_unique<xgQuatInterpolator>(name);
	else if (type == "xgBone") 					return std::make_unique<xgBone>(name);
	else if (type == "xgBgMatrix")				return std::make_unique<xgBgMatrix>(name);
	else if (type == "xgEnvelope") 				return std::make_unique<xgEnvelope>(name);
	else if (type == "xgMaterial") 				return std::make_unique<xgMaterial>(name);
	else if (type == "xgTexture") 				return std::make_unique<xgTexture>(name);
	else if (type == "xgDagMesh") 				return std::make_unique<xgDagMesh>(name);
	else if (type == "xgBgGeometry") 			return std::make_unique<xgBgGeometry>(name);
	else if (type == "xgDagTransform") 			return std::make_unique<xgDagTransform>(name);
	else if (type == "xgMultiPassMaterial") 	return std::make_unique<xgMultiPassMaterial>(name);
	else if (type == "xgVertexInterpolator") 	return std::make_unique<xgVertexInterpolator>(name);
	else if (type == "xgNormalInterpolator") 	return std::make_unique<xgNormalInterpolator>(name);
	else if (type == "xgShapeInterpolator") 	return std::make_unique<xgShapeInterpolator>(name);
	else if (type == "xgTexCoordInterpolator")	return std::make_unique<xgTexCoordInterpolator>(name);
	else if (type == "xgTime")					return std::make_unique<xgTime>(name);
	else
		throw "Unrecognized node";
}

XG_SubNode* XG::searchForNode(FileReader& file) const
{
	return searchForNode(PString::GetString(file));
}

XG_SubNode* XG::searchForNode(std::string_view name) const
{
	for (const auto& node : m_nodes)
		if (node->getName() == name)
			return node.get();
	return nullptr;
}

XG_SubNode* XG::grabNode_optional(std::string_view inputString, std::string_view outputString, FileReader& file) const
{
	if (!PString::CheckForString(inputString, file))
		return nullptr;

	XG_SubNode* node = searchForNode(file);
	if (node == nullptr)
		throw "Node not located";

	if (!PString::CheckForString(outputString, file))
		throw "Output string not matched";

	return node;
}

XG_SubNode* XG::grabNode(std::string_view inputString, std::string_view outputString, FileReader& file) const
{
	XG_SubNode* node = grabNode_optional(inputString, outputString, file);
	if (node == nullptr)
		throw "Input string not matched";
	return node;
}

void XG::fillDag(DagElement& dag, FileReader& file)
{
	while (!PString::CheckForString("]", file))
	{
		DagElement& newDag = dag.m_connections.emplace_back(searchForNode(file));
		if (PString::CheckForString("[", file))
			fillDag(newDag, file);
	}
}

void XG::saveDag(const DagElement& dag, FileWriter& file, bool forceBrackets) const
{
	if (dag.m_mesh)
		dag.m_mesh->writeName(file);
	else
		dag.m_transform->writeName(file);

	if (!dag.m_connections.empty() || forceBrackets)
	{
		PString::WriteString("[", file);
		for (const auto& connection : dag.m_connections)
			saveDag(connection, file, false);
		PString::WriteString("]", file);
	}
}
