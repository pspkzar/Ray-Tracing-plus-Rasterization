#ifndef EVENTFACTORY_H
#define EVENTFACTORY_H

#include <curitiba/event/iEventData.h>
#include <string>

namespace curitiba
{
	namespace event_
	{
		class EventFactory
		{
		public:
			static curitiba::event_::IEventData* create (std::string type);
		private:
			EventFactory(void) {};
			~EventFactory(void) {};
		};
	};
};
#endif