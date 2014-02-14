#ifndef PHYSICALOBJECT_H
#define PHYSICALOBJECT_H

#include <vector>

#include <curitiba/scene/sceneobject.h>

using namespace curitiba::scene;

namespace curitiba
{
	namespace world
	{
		class PhysicalObject
		{
		private:
			SceneObject *m_SceneObject;
		public:
			PhysicalObject (void);
		
			virtual ~PhysicalObject (void);
		};	
	};
};
#endif
