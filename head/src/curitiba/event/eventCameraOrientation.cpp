#include <curitiba/event/eventCameraOrientation.h>

using namespace curitiba::event_;

EventCameraOrientation::EventCameraOrientation(curitiba::event_::CameraOrientation cam)
{
	this->cam=cam;
}

EventCameraOrientation::EventCameraOrientation(const EventCameraOrientation &c)
{
	cam=c.cam;
}



EventCameraOrientation::EventCameraOrientation(void)
{
	cam.setCameraOrientation(0,0,0,0,0,0,0);
}

EventCameraOrientation::~EventCameraOrientation(void)
{
}

void EventCameraOrientation::setData(void *data)
{
	curitiba::event_::CameraOrientation *d=(curitiba::event_::CameraOrientation *)data;
	this->cam=*d;
}


void *EventCameraOrientation::getData(void)
{
	return &cam;
}