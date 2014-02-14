#ifndef GLTEXTUREMS_H
#define GLTEXTUREMS_H

#include <curitiba/render/opengl/glTexture.h>

using namespace curitiba::render;
using namespace curitiba::material;

namespace curitiba
{
	namespace render
	{
		class GLTextureMS : public GLTexture
		{
		friend class Texture;

		public:

			
		protected:

			GLTextureMS (std::string label, std::string internalFormat,
				int width, int height, int samples );

		public:
			~GLTextureMS(void);
		};
	};
};
#endif
