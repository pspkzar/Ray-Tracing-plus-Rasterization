#ifndef LISTENER_H
#define LISTENER_H

#include <string>
#include <curitiba/event/iEventData.h>

namespace curitiba
{
	namespace event_
	{
		class IListener
		{
		public:

			// Construtors
			IListener(void){};
			~IListener(void){};

			// Methods
			virtual std::string &getName () = 0;
			virtual void eventReceived(const std::string &sender, const std::string &eventType, IEventData *evt){};
		};
	};
};
#endif