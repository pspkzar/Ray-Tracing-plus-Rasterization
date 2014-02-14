#include <curitiba/render/renderqueuefactory.h>
#include <curitiba/render/materialsortrenderqueue.h>

using namespace curitiba::render;

IRenderQueue*
RenderQueueFactory::create (std::string queueType)
{
	if ("MaterialSort" == queueType) {
		return new MaterialSortRenderQueue;
	}
	return 0;
}
