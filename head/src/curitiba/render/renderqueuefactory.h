#ifndef RENDERQUEUEFACTORY_H
#define RENDERQUEUEFACTORY_H

#include <string>

#include <curitiba/render/irenderqueue.h>

namespace curitiba
{
	namespace render
	{
		class RenderQueueFactory
		{
		public:
			static IRenderQueue* create (std::string renderType);
		private:
			RenderQueueFactory(void);
			~RenderQueueFactory(void);
		};
	};
};

#endif //RENDERQUEUEFACTORY_H

