#ifndef EventCameraMotion_H
#define EventCameraMotion_H

#include <curitiba/event/iEventData.h>
#include <curitiba/event/cameraMotion.h>

namespace curitiba
{
	namespace event_
	{
		class EventCameraMotion : public IEventData
		{
		public:
			curitiba::event_::CameraMotion cam;
			
			EventCameraMotion(curitiba::event_::CameraMotion cam);
			EventCameraMotion(const EventCameraMotion &c);
			EventCameraMotion(void);
			~EventCameraMotion(void);
			
			void setData(void *data);
			void *getData(void);			
		};
	};
};

#endif