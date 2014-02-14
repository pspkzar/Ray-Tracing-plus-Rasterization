#include <curitiba/event/eventCameraMotion.h>

using namespace curitiba::event_;

EventCameraMotion::EventCameraMotion(curitiba::event_::CameraMotion cam)
{
	this->cam=cam;	
}

EventCameraMotion::EventCameraMotion(const EventCameraMotion &c)
{
	cam=c.cam;
}



EventCameraMotion::EventCameraMotion(void)
{
	cam.setCameraMotion("",0);
}

EventCameraMotion::~EventCameraMotion(void)
{
}

void EventCameraMotion::setData(void *data)
{
	curitiba::event_::CameraMotion *d=(curitiba::event_::CameraMotion *)data;
	this->cam=*d;
}


void *EventCameraMotion::getData(void)
{
	return &cam;
}