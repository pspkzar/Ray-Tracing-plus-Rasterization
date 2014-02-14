#include <curitiba/render/iprogram.h>
#include <curitiba/config.h>

#ifdef CURITIBA_OPENGL
#include <curitiba/render/opengl/glprogram.h>
#endif

using namespace curitiba::render;

IProgram*
IProgram::create (void) 
{
#ifdef CURITIBA_OPENGL
	return new GlProgram;
#elif CURITIBA_DIRECTX
	return new DXProgram;
#endif
}
