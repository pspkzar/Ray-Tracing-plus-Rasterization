#include <curitiba/geometry/meshbones.h>
#include <curitiba/material/imaterialgroup.h>
#include <curitiba/material/materialgroup.h>
#include <curitiba/render/irenderable.h>
#include <curitiba/render/vertexdata.h>
#include <curitiba/math/vec3.h>

//#include <algorithm>

using namespace curitiba::geometry;
using namespace curitiba::render;
using namespace curitiba::material;
using namespace curitiba::math;

MeshBones::MeshBones(void) : Mesh()
{
}

MeshBones::~MeshBones(void)
{
}


void 
MeshBones::addBoneWeight(unsigned int vertex, unsigned int bone, float weight)
{
	if (!m_BoneAssignments.count(vertex))
		m_BoneAssignments[vertex] = BoneWeights();

	m_BoneAssignments[vertex].push_back(std::pair<unsigned int, float>(bone,weight));
	
}

std::string 
MeshBones::getType()
{
	return("MeshBones");
}