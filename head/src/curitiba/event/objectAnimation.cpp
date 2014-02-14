#include <curitiba/event/objectAnimation.h>
#include <curitiba/event/eventFactory.h>
#include <curitiba.h>

ObjectAnimation::ObjectAnimation(std::string name, SceneObject *object)
{
	this->name=name;
	this->object=object;
	object->_getTransformPtr()->setIdentity();
	this->addAnimationListener();
}

ObjectAnimation::ObjectAnimation(void)
{
	name="";
	object=0;
}

ObjectAnimation::ObjectAnimation(const ObjectAnimation &c)
{
	name=c.name;
	object=c.object;
}

ObjectAnimation::~ObjectAnimation(void)
{
}

std::string &ObjectAnimation::getName(void){
	return name;
}

void ObjectAnimation::removeAnimationListener(void){
	curitiba::event_::IListener *lst;
	lst = this;
	EVENTMANAGER->removeListener("OBJECT_FRACTION",lst);
}

void ObjectAnimation::addAnimationListener(void){
	curitiba::event_::IListener *lst;
	lst = this;
	EVENTMANAGER->addListener("OBJECT_FRACTION",lst);
}

SceneObject *ObjectAnimation::getObject(void){
	return object;
}

void ObjectAnimation::eventReceived(const std::string &sender, const std::string &eventType, curitiba::event_::IEventData *evt){ 
		
	vec3 *f=(vec3 *)evt->getData();
	vec3 fc=*f;

	if(object!=0){
				//verificar na animação como mudar a posição de um objecto polySurface1219
		object->_getTransformPtr()->translate (fc);
	}	
	curitiba::event_::IEventData *e= curitiba::event_::EventFactory::create("Vector");
	e->setData(&fc);
	EVENTMANAGER->notifyEvent("OBJECT_POSITION", &name[0], "", e);
	delete e;
	return;
			
}

//void ObjectAnimation::init(std::string name, ISceneObject *o){
//	this->name=name;
//	this->object=o;
//	this->addAnimationListener();
//}