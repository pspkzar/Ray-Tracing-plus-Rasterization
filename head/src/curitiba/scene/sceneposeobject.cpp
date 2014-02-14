#include <curitiba/scene/sceneposeobject.h>
#include <curitiba/geometry/boundingbox.h>
#include <curitiba/math/transformfactory.h>
#include <curitiba/geometry/meshwithpose.h>

using namespace curitiba::scene;
using namespace curitiba::render;
using namespace curitiba::geometry;
using namespace curitiba::math;

ScenePoseObject::ScenePoseObject(void) : SceneObject()
{
	m_StaticCondition = false;
}

ScenePoseObject::~ScenePoseObject(void) {

}

bool
ScenePoseObject::isStatic() {

	return false;
}


void 
ScenePoseObject::setStaticCondition(bool aCondition) {

}


const IBoundingVolume*
ScenePoseObject::getBoundingVolume ()
{
	if (0 == m_BoundingVolume) {
		calculateBoundingVolume();
		if (m_Transform)
			m_BoundingVolume->setTransform (*m_Transform);
	}
	//m_BoundingVolume->transform (*m_Transform);
	return (m_BoundingVolume);
}


// When one wants to manually set the bounding volume
// for instance for a node in an octree
// has no effect for pose based objects
void
ScenePoseObject::setBoundingVolume (IBoundingVolume *b)
{
}



// This is a NOP for Pose based objects
void 
ScenePoseObject::burnTransform (void)
{

}


std::string 
ScenePoseObject::getType (void)
{
	return "PoseObject";
}


void
ScenePoseObject::calculateBoundingVolume (void)
{
	if (0 != m_BoundingVolume) {
		delete m_BoundingVolume;
	}
	m_BoundingVolume = new BoundingBox; /***MARK***/

	MeshPose *mp = (MeshPose *)m_Renderable;

	mp->setReferencePose();
	m_BoundingVolume->calculate (m_Renderable->getVertexData().getDataOf (VertexData::getAttribIndex("position")));
	
	IBoundingVolume *bv = new BoundingBox;

	unsigned int numPoses = mp->getNumberOfPoses();
	for (unsigned int i = 0; i < numPoses; i++) {

		mp->setPose(i);
		bv->calculate( m_Renderable->getVertexData().getDataOf (VertexData::getAttribIndex("position")));
		m_BoundingVolume->compound(bv);
	}
	mp->setReferencePose();
}


void 
ScenePoseObject::writeSpecificData (std::fstream &f)
{
	return;
}


void 
ScenePoseObject::readSpecificData (std::fstream &f)
{
	return;
}




// Does not apply
void ScenePoseObject::unitize(float min, float max) {
	
}