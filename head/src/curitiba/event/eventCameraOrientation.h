#ifndef EventCameraOrientation_H
#define EventCameraOrientation_H

#include <curitiba/event/iEventData.h>
#include <curitiba/event/cameraOrientation.h>

namespace curitiba
{
	namespace event_
	{
		class EventCameraOrientation : public IEventData
		{
		public:
			curitiba::event_::CameraOrientation cam;
			
			EventCameraOrientation(curitiba::event_::CameraOrientation cam);
			EventCameraOrientation(const EventCameraOrientation &c);
			EventCameraOrientation(void);
			~EventCameraOrientation(void);
			
			void setData(void *data);
			void *getData(void);

		};
	};
};

#endif