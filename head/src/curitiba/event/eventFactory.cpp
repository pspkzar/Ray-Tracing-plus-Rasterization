#include <curitiba/event/eventFactory.h>
#include <curitiba/event/eventVec3.h>
#include <curitiba/event/eventVec4.h>
#include <curitiba/event/eventString.h>
#include <curitiba/event/eventFloat.h>
#include <curitiba/event/eventInt.h>
#include <curitiba/event/eventCameraOrientation.h>
#include <curitiba/event/eventCameraMotion.h>
#include <assert.h>

using namespace curitiba::event_;
using namespace std;

curitiba::event_::IEventData* 
EventFactory::create (std::string type)
{
	if ("String" == type) {
		return new curitiba::event_::EventString;
	}

	if ("Vec3" == type) {
		return new curitiba::event_::EventVec3;
	}

	if ("Vec4" == type) {
		return new curitiba::event_::EventVec4;
	}

	if ("Float" == type) {
		return new curitiba::event_::EventFloat;
	}

	if ("Camera Orientation" == type) {
		return new curitiba::event_::EventCameraOrientation;
	}

	if ("Camera Motion" == type) {
		return new curitiba::event_::EventCameraMotion;
	}
	if ("Int" == type) {
		return new curitiba::event_::EventInt;
	}

	assert("EventFactory: invalid type");
	return 0;
}