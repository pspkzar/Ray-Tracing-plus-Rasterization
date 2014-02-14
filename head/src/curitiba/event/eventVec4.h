#ifndef EventVec4_H
#define EventVec4_H

#include <curitiba/event/iEventData.h>
#include <curitiba/event/eventvec4.h>
#include <curitiba/math/vec4.h>

namespace curitiba
{
	namespace event_
	{
		class EventVec4: public IEventData
		{
		public:
			curitiba::math::vec4 v;

			EventVec4(curitiba::math::vec4 v);
			EventVec4(const EventVec4 &c);
			EventVec4(void);
			~EventVec4(void);
			
			void setData(void *data);
			void *getData(void);

		};
	};
};

#endif