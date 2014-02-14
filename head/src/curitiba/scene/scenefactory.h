#ifndef SCENEFACTORY_H
#define SCENEFACTORY_H

#include <curitiba/scene/iscene.h>

namespace curitiba
{
	namespace scene
	{
		class SceneFactory
		{
		public:
			static IScene * create (std::string scene);
		private:
			SceneFactory(void) {};
		};
	};
};

#endif //SCENEFACTORY_H
