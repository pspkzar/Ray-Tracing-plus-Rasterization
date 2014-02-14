#ifndef LIGHTWITHSWITCH_H
#define LIGHTWITHSWITCH_H

// Marta
#include <curitiba/scene/light.h>
#include <curitiba/event/iEventData.h>
#include <curitiba/event/ilistener.h>

namespace curitiba
{
	namespace scene
	{
		class LightWithSwitch: public Light
		{
		public:

			std::string name;
			LightWithSwitch (std::string &name);
			~LightWithSwitch(void);

			std::string& getName (void);

			void lightOff(void);
			void lightOn(void);
			void addLightListener(void);
			void removeLightListener(void);
			void eventReceived(const std::string &sender, const std::string &eventType, curitiba::event_::IEventData *evt);
		};
	};
};
#endif