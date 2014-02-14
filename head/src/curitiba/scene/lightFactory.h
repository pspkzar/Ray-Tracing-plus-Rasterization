// Marta
#ifndef LIGHTFACTORY_H
#define LIGHTFACTORY_H

#include <curitiba/scene/light.h>

namespace curitiba
{
	namespace scene
	{
		class LightFactory
		{
		public:
			static Light *create ( std::string lName, std::string lType);
		private:
			LightFactory(void) {};
		};
	};
};

#endif //LIGHTFACTORY_H