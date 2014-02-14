#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <curitiba/scene/iscene.h>

namespace curitiba 
{

	namespace loader 
	{
		// Wavefront OBJ format loader
		// Uses Nate Robin's GLM implementation as the core
		class OBJLoader
		{
		public:
			// Load Scene
			static void loadScene (curitiba::scene::IScene *aScene, std::string &aFilename);
			// Write Scene
			static void writeScene (curitiba::scene::IScene *aScene, std::string &aFilename);

		private:
			// Constructor
			OBJLoader(void) {};
			// Destructor
			~OBJLoader(void) {};
		};
	};
};

#endif //OBJLOADER_H