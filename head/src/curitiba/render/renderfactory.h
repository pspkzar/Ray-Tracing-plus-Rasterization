#ifndef RENDERFACTORY
#define RENDERFACTORY

#include <curitiba/render/irenderer.h>

namespace curitiba
{
	namespace render
	{
		class RenderFactory
		{
		public:
			static IRenderer* create (void);
		private:
			RenderFactory (void) {};
		};
	};
};

#endif //RENDERFACTORY
