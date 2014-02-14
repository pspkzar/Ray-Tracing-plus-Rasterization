#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>

#include <curitiba/render/texture.h>
#include <curitiba/material/teximage.h>

using namespace curitiba::material;

namespace curitiba
{
	namespace loader
	{
		class TextureLoader
		{
		public:
			static TextureLoader* create (void);
		public:
			virtual int loadImage (std::string file) = 0;
			virtual unsigned char* getData (void) = 0;
			virtual int getWidth (void) = 0;
			virtual int getHeight (void) = 0;
			virtual std::string getFormat (void) = 0; 
			virtual std::string getType (void) = 0;
			virtual void freeImage (void) = 0;

			virtual void save(TexImage *ti, std::string filename) = 0;

		public:
			virtual ~TextureLoader(void) {};
		};
	};
};

#endif //TEXTURELOADER_H
