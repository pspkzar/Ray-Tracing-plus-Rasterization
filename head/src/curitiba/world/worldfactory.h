#ifndef WORLDFACTORY_H
#define WORLDFACTORY_H

#include <curitiba/world/iworld.h>

namespace curitiba
{
	namespace world
	{
		class WorldFactory
		{
		public:
			static curitiba::world::IWorld* create (std::string type);
		private:
			WorldFactory(void) {};
			~WorldFactory(void) {};
		};
	};
};
#endif
