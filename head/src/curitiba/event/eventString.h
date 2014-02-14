#ifndef EventString_H
#define EventString_H

#include <curitiba/event/iEventData.h>
#include <string>

namespace curitiba
{
	namespace event_
	{
		class EventString : public IEventData
		{
		public:
			std::string *str;

			EventString(std::string *str);
			EventString(const EventString &s);
			EventString(void);
			~EventString(void);
			
			void setData(void *data);			
			void *getData(void);

		};
	};
};

#endif