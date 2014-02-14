#include <curitiba/world/physicalobject.h>

using namespace curitiba::world;

PhysicalObject::PhysicalObject():
	m_SceneObject (0)
{
}

PhysicalObject::~PhysicalObject()
{
	delete m_SceneObject;
}
