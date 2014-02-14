#include <curitiba/event/eventVec3.h>


using namespace curitiba::event_;

EventVec3::EventVec3(curitiba::math::vec3 v)
{
	this->v = v;
}


EventVec3::EventVec3(const EventVec3 &c)
{
	v=c.v;
}


EventVec3::EventVec3(void)
{
	v.set(0.0f, 0.0f, 0.0f);
}

EventVec3::~EventVec3(void)
{
}


void EventVec3::setData(void *data) {
	
	curitiba::math::vec3 *ve=(curitiba::math::vec3 *)data;
	this->v=*ve;
}

void *EventVec3::getData(void) {
	return &v;
}



