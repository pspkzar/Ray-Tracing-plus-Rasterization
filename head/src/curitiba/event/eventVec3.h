#ifndef EventVec3_H
#define EventVec3_H

#include <curitiba/event/iEventData.h>
#include <curitiba/math/vec3.h>

namespace curitiba
{
	namespace event_
	{
		class EventVec3: public IEventData
		{
		public:
			curitiba::math::vec3 v;

			EventVec3(curitiba::math::vec3 v);
			EventVec3(const EventVec3 &c);
			EventVec3(void);
			~EventVec3(void);
			
			void setData(void *data);
			void *getData(void);

		};
	};
};

#endif