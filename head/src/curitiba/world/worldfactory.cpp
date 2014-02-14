#include <curitiba/world/worldfactory.h>

#include <curitiba/world/bulletworld.h>

using namespace curitiba::world;

curitiba::world::IWorld* 
WorldFactory::create (std::string type)
{
	if ("Bullet" == type) {
		return new BulletWorld;
	}

	return 0;
}
