#ifndef GLTEXIMAGE_H
#define GLTEXIMAGE_H

#include <curitiba/render/opengl/gltexture.h>
#include <curitiba/material/teximage.h>

#include <GL/glew.h>

using namespace curitiba::render;

namespace curitiba
{
	namespace render
	{
		class GLTexImage : public curitiba::material::TexImage
		{
		friend class curitiba::material::TexImage;

		public:

			virtual void update(void);
			virtual void *getData();


		protected:
			GLTexImage (Texture *t);
			~GLTexImage(void);




		};
	};
};
#endif
