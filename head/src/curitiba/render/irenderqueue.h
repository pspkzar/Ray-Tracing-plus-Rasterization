#ifndef IRENDERQUEUE_H
#define IRENDERQUEUE_H

#include <curitiba/material/materialid.h>
#include <curitiba/scene/sceneobject.h>

namespace curitiba
{
	namespace render
	{
		class IRenderQueue
		{
		public:
			virtual void clearQueue (void) = 0;
			virtual void addToQueue (curitiba::scene::SceneObject* aObject,
				std::map<std::string, curitiba::material::MaterialID> &materialMap) = 0;
			virtual void processQueue (void) = 0;

			virtual ~IRenderQueue(void) {};
		};
	};
};

#endif //IRENDERQUEUE_H
