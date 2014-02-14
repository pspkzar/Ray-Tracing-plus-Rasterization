#include <curitiba/render/renderfactory.h>
#include <curitiba/render/opengl/glrenderer.h>
#include <curitiba/config.h>

using namespace curitiba::render;

IRenderer* 
RenderFactory::create (void)
{
#ifdef CURITIBA_OPENGL
	return new GLRenderer;
#elif CURITIBA_DIRECTX
	return new DXRenderer;
#else
	return 0;
#endif
}
