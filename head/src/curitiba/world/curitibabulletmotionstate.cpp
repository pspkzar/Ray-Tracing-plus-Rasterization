#include <curitiba/world/curitibabulletmotionstate.h>
#include <curitiba/math/transformfactory.h>

using namespace curitiba::world;

CuritibaBulletMotionState::CuritibaBulletMotionState (curitiba::scene::SceneObject *aObject) : m_SceneObject (aObject)
{
	t = TransformFactory::create("SimpleTransform");
}

CuritibaBulletMotionState::~CuritibaBulletMotionState(void)
{
}

void 
CuritibaBulletMotionState::getWorldTransform (btTransform &worldTrans) const
{
	worldTrans.setFromOpenGLMatrix (m_SceneObject->getTransform().getMat44().getMatrix());
}

// updates scene object transform
// caution, must not access directly the transform
void 
CuritibaBulletMotionState::setWorldTransform (const btTransform &worldTrans)
{
//	worldTrans.getOpenGLMatrix (const_cast<float*> (m_SceneObject->getTransform().getMat44().getMatrix()));
	worldTrans.getOpenGLMatrix (const_cast<float*> (t->getMat44().getMatrix()));
	m_SceneObject->setTransform(t);
}
