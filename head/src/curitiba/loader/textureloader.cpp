#include <curitiba/loader/textureloader.h>

#include <curitiba/loader/deviltextureloader.h>

using namespace curitiba::loader;

TextureLoader*
TextureLoader::create (void)
{
	return new DevILTextureLoader();
}
