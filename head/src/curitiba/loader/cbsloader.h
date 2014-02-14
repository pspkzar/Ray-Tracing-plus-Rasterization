#ifndef CBSLOADER_H
#define CBSLOADER_H

#include <curitiba/scene/iscene.h>

namespace curitiba 
{

	namespace loader 
	{
		class CBSLoader
		{
		public:
			static void loadScene (curitiba::scene::IScene *aScene, std::string &aFilename);
			static void writeScene (curitiba::scene::IScene *aScene, std::string &aFilename);

		private:
			CBSLoader(void) {};
			~CBSLoader(void) {};
		};
	};
};

#endif //CBSLOADER_H
