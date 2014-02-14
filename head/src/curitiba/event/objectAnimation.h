#ifndef OBJECTANIMATION_H
#define OBJECTANIMATION_H

#include <curitiba/event/ilistener.h>
#include <curitiba/event/iEventData.h>
#include <curitiba/math/vec3.h>
#include <curitiba/scene/sceneobject.h>

using namespace curitiba::scene;

namespace curitiba
{
	namespace event_
	{
		class ObjectAnimation: public IListener
		{
		public:
			std::string name;
			SceneObject *object;

			ObjectAnimation(std::string name, SceneObject *object);
			ObjectAnimation(const ObjectAnimation &c);
			ObjectAnimation(void);
			~ObjectAnimation(void);

			std::string &getName(void);
			void removeAnimationListener(void);
			void addAnimationListener(void);
			SceneObject *getObject(void);		
			void eventReceived(const std::string &sender, const std::string &eventType, curitiba::event_::IEventData *evt);
			//void init(std::string name, ISceneObject *o);
		};
	};
};

#endif
