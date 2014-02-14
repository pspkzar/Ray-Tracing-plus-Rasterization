#include <curitiba/event/eventVec4.h>


using namespace curitiba::event_;

EventVec4::EventVec4(curitiba::math::vec4 v)
{
	this->v = v;
}


EventVec4::EventVec4(const EventVec4 &c)
{
	v=c.v;
}


EventVec4::EventVec4(void)
{
	v.set(0.0f, 0.0f, 0.0f, 0.0f);
}


EventVec4::~EventVec4(void)
{
}


void EventVec4::setData(void *data) {
	
	curitiba::math::vec4 *ve=(curitiba::math::vec4 *)data;
	this->v=*ve;
}


void *EventVec4::getData(void) {
	return &v;
}



