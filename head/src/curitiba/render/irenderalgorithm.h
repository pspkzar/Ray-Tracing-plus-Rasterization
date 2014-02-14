#ifndef IRENDERALGORITHM
#define IRENDERALGORITHM

#include <curitiba/scene/iscene.h>
#include <curitiba/render/irenderer.h>

namespace curitiba 
{
	namespace render
	{
		class IRenderAlgorithm
		{
		public:
			virtual void renderScene (curitiba::scene::IScene* aScene) = 0;
			virtual void setRenderer (curitiba::render::IRenderer *aRenderer) = 0;
			virtual void init (void) = 0;

			virtual void externCommand (char keyCode) = 0;
		public:
			virtual ~IRenderAlgorithm(void) {};
		};
	};
};

#endif
