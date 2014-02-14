#include <curitiba/scene/lightWithSwitch.h>
#include <curitiba.h>

using namespace curitiba::scene;
using namespace curitiba::math;
using namespace curitiba;

LightWithSwitch::LightWithSwitch(std::string &name): Light(name)
{
	this->name=name;
	this->addLightListener(); 
}

LightWithSwitch::~LightWithSwitch(void)
{
}

std::string& LightWithSwitch::getName (void){
	
	return name;

}

void LightWithSwitch::lightOff(){

	this->m_BoolProps[ENABLED] = false;
}

void LightWithSwitch::lightOn(){

	this->m_BoolProps[ENABLED] = true;
}

void LightWithSwitch::addLightListener(void)
{
	//curitiba::event_::IListener *lst;
	//lst = this;
	EVENTMANAGER->addListener("LIGHT_ON",(curitiba::event_::IListener *)this);
	EVENTMANAGER->addListener("LIGHT_OFF",(curitiba::event_::IListener *)this);
}

void LightWithSwitch::removeLightListener(void)
{
	//curitiba::event_::IListener *lst;
	//lst = this;
	EVENTMANAGER->removeListener("LIGHT_ON",(curitiba::event_::IListener *)this);
	EVENTMANAGER->addListener("LIGHT_OFF",(curitiba::event_::IListener *)this);
}

void LightWithSwitch::eventReceived(const std::string &sender, const std::string &eventType, curitiba::event_::IEventData *evt){
	//if (evt->getReceiver()==this->getName()){
		if (eventType == "LIGHT_ON")
			lightOn();
		if (eventType == "LIGHT_OFF")
			lightOff();
	//}
}