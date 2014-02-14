#ifndef DEVILTEXTURELOADER_H
#define DEVILTEXTURELOADER_H

#include <curitiba/loader/textureloader.h>
#include <curitiba/material/teximage.h>

using namespace curitiba::material;

#include <IL/il.h>

namespace curitiba
{
	namespace loader
	{
		class DevILTextureLoader : public TextureLoader
		{
		friend class TextureLoader;

		protected:
			DevILTextureLoader (void);
		public:
			~DevILTextureLoader (void);

			int loadImage (std::string file);
			unsigned char* getData (void);
			int getWidth (void);
			int getHeight (void);
			std::string getFormat (void); 
			std::string getType (void);
			void freeImage (void);

			virtual void save(TexImage *ti, std::string filename);

		private:
			ILuint m_IlId;
			static bool inited;	
		};
	};
};

#endif //DEVILTEXTURELOADER_H
