#include <curitiba/event/sensorFactory.h>
//#include <curitiba/event/touchSensor.h>
#include <curitiba/event/proximitySensor.h>
#include <curitiba/event/timeSensor.h>

using namespace curitiba::event_;
using namespace std;

curitiba::event_::Sensor* 
SensorFactory::create (std::string type)
{
	if ("ProximitySensor" == type) {
		return new curitiba::event_::ProximitySensor;
	}

/*	if("TouchSensor" == type) {
		return new curitiba::event_::TouchSensor;
	}
*/
	if("TimeSensor" == type) {
		return new curitiba::event_::TimeSensor;
	}

	return 0;
}

bool
SensorFactory::validate(std::string type)
{
	if (("ProximitySensor" == type) || ("TimeSensor" == type))
		return true;
	else
		return false;

}