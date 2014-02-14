#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <string>

namespace curitiba
{
	namespace loader
	{
		class SceneLoader
		{
		public:
			static bool load (std::string file);
		private:
			SceneLoader (void);
		};
	};
};
#endif //SCENELOADER_H
