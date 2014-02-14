#include <curitiba/scene/sceneobjectfactory.h>

#include <curitiba/scene/sceneobject.h>
#include <curitiba/scene/octreenode.h>
#include <curitiba/scene/geometryobject.h>
#include <curitiba.h>

using namespace curitiba::scene;

SceneObject* 
SceneObjectFactory::create (std::string type)
{
	SceneObject *s;
	if (0 == type.compare ("SimpleObject")) {
		s = new SceneObject;
	} 

	else if (0 == type.compare ("OctreeNode")) {
		s =  new OctreeNode;
	}

	else if (0 == type.compare ("Geometry")) {
		s =  new GeometricObject;
	}
	else {
		assert("SceneObjectFactory: type is not valid");
		return 0;
	}

	RENDERMANAGER->addSceneObject(s);
	return s;
}
