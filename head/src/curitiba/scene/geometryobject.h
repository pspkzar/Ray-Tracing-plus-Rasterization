#ifndef GEOMETRYOBJECT_H
#define GEOMETRYOBJECT_H

/* These are very similiar to SceneObjects. It just simplifies the 
 material settings, since primitives have a single material with a fixed name */


#include <curitiba/scene/sceneobject.h>
#include <curitiba/scene/sceneobjectfactory.h>

namespace curitiba
{
	namespace scene
	{
		class GeometricObject : public curitiba::scene::SceneObject
		{
		public:
			friend class curitiba::scene::SceneObjectFactory;

			~GeometricObject(void);

			static unsigned int PrimitiveCounter;

			unsigned int m_PrimitiveID;
			void setRenderable(curitiba::render::IRenderable *renderable);
			void setMaterial(const std::string &name);

			std::string getType (void);

		protected:
			GeometricObject();
		};
	};
};
#endif