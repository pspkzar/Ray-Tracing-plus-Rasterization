#ifndef SCENEOBJECTFACTORY_H
#define SCENEOBJECTFACTORY_H

//#include <curitiba/scene/sceneobject.h>

#include <string>


namespace curitiba
{
	namespace scene
	{
		class SceneObject;

		class SceneObjectFactory
		{
		public:
			static curitiba::scene::SceneObject* create (std::string type);
		private:
			SceneObjectFactory(void) {};
			~SceneObjectFactory(void) {};
		};
	};
};
#endif
