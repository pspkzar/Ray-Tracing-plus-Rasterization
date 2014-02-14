// Marta
#include <curitiba/scene/lightFactory.h>
#include <curitiba/scene/lightWithSwitch.h>


using namespace curitiba::scene;


Light*
LightFactory::create( std::string lName, std::string lType)
{
	if ("default"== lType){
		return new Light(lName);
	}

	if ("Light" == lType) {
		return new Light(lName);
	}
	
	if ("LightWithSwitch" == lType) {
		return new LightWithSwitch(lName);
	}
	return 0;
}