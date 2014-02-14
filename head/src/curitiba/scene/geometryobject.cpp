#include <curitiba/scene/geometryobject.h>
#include <curitiba/geometry/box.h>
#include <curitiba.h>
#include <curitiba/material/material.h>
#include <sstream>

using namespace curitiba::scene;
using namespace curitiba::geometry;


unsigned int GeometricObject::PrimitiveCounter = 0;

GeometricObject::GeometricObject() : SceneObject()
{
	m_PrimitiveID = GeometricObject::PrimitiveCounter;
	++GeometricObject::PrimitiveCounter;

	std::stringstream z;
	z << "GeomObj";
	z << m_PrimitiveID;
	
	this->setName(z.str());
}


GeometricObject::~GeometricObject()
{
	
}


void 
GeometricObject::setRenderable (curitiba::render::IRenderable *renderable)
{
	// It must be a primitive!
	m_Renderable = renderable;
}


void 
GeometricObject::setMaterial(const std::string &name) 
{
	IMaterialGroup *mg = m_Renderable->getMaterialGroups().at(0);

	mg ->setMaterialName(name);
}


std::string 
GeometricObject::getType (void)
{
	return "GeometricPrimitive";
}