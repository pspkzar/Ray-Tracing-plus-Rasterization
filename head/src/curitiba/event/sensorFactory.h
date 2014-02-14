#ifndef SENSORFACTORY_H
#define SENSORFACTORY_H

#include <curitiba/event/sensor.h>
#include <string>

namespace curitiba
{
	namespace event_
	{
		class SensorFactory
		{
		public:
			static curitiba::event_::Sensor* create (std::string type);
			static bool validate(std::string type);
		private:
			SensorFactory(void) {};
			~SensorFactory(void) {};
		};
	};
};
#endif