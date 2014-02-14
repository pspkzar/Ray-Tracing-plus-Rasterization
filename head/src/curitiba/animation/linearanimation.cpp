#include <curitiba/animation/linearanimation.h>

#include <curitiba/math/vec3.h>
#include <curitiba/geometry/iboundingvolume.h>

using namespace curitiba::animation;
using namespace curitiba::math;
using namespace curitiba::geometry;

LinearAnimation::LinearAnimation(curitiba::scene::ISceneObject *aObject, curitiba::math::vec3 start, curitiba::math::vec3 end) :
	m_SceneObject (aObject), m_StartPos (start), m_LineVector (end),  m_CurrentPos (start), m_LocalTime (0.0f)
{
	m_LineVector -= m_StartPos;
	//m_LineVector.normalize();

	m_SceneObject->_getTransformPtr()->setIdentity();
	m_SceneObject->_getTransformPtr()->translate (start);
}

LinearAnimation::LinearAnimation(curitiba::scene::ISceneObject *aObject, curitiba::math::vec3 end) :
	m_SceneObject (aObject), m_LineVector (end), m_LocalTime (0.0f)
{
	m_StartPos.set (0.0f, 0.0f, 0.0f);
	m_CurrentPos.set (0.0f, 0.0f, 0.0f);

	m_LineVector -= m_StartPos;
	//m_LineVector.normalize();
}


void 
LinearAnimation::step (float deltaT)
{
	if (true == isFinished()) {
		return;
	}
	
	m_LocalTime += (deltaT *  0.0001f);
	
	vec3 position (m_StartPos);

	vec3 aux (m_LineVector);

	aux *= m_LocalTime;

	position += aux;

	vec3 increment (position);

	increment -= m_CurrentPos;

	m_CurrentPos.set (position.x, position.y, position.z);

	m_SceneObject->_getTransformPtr()->translate (increment);
}

bool
LinearAnimation::isFinished (void)
{
	if (m_LocalTime >= 1.0f) {
		return true;
	}
	return false;
}


