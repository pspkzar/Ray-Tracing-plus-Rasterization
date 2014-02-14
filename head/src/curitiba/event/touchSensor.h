#ifndef TOUCHSENSOR_H
#define TOUCHSENSOR_H

#include <curitiba/event/iEventData.h>
#include <curitiba/event/sensor.h>
#include <curitiba/math/vec3.h>
#include <curitiba/scene/sceneobject.h>


using namespace curitiba::math;
using namespace curitiba::scene;

namespace curitiba
{
	namespace event_
	{
			class TouchSensor: public Sensor
			{
			public:
				std::string name;
				ISceneObject *object;
				bool enabled;
				curitiba::math::vec3 colorSensitive;


				TouchSensor(std::string name, ISceneObject *object, bool enabled, curitiba::math::vec3 colorSensitive); 
				TouchSensor(const TouchSensor &c);
				TouchSensor(void);
				~TouchSensor(void);

				void setTouchSensor(std::string name, ISceneObject *object, bool enabled, curitiba::math::vec3 colorSensitive);
				std::string &getName(void);
				ISceneObject *getObject(void);
				bool getEnabled(void);
				curitiba::math::vec3 getColorSensitive(void);
				bool isSelected(int x, int y);
				void removeTouchListener(void);
				void addTouchListener(void);
				void eventReceived(const std::string &sender, const std::string &eventType, curitiba::event_::IEventData *evt);
				void init(std::string name, bool enabled, std::string str);
			};
	};
};
#endif