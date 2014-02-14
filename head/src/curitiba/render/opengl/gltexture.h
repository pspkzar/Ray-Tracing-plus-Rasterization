#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <curitiba/render/texture.h>
//#include <curitiba/material/textureSampler.h>
#include <curitiba/render/opengl/gltexturesampler.h>

#include <GL/glew.h>

using namespace curitiba::render;
using namespace curitiba::material;

namespace curitiba
{
	namespace render
	{
		class GLTexture : public Texture
		{
		friend class Texture;

		public:

			~GLTexture(void);

			static std::string StringType[];
			static int Type[];

			static std::string StringDimension[];
			static int Dimension[];

			static std::string StringFormat[];
			static int Format[];

			static std::string StringInternalFormat[];
			static int InternalFormat[];

			static bool isValidFormat(std::string aFormat);
			static bool isValidInternalFormat(std::string anInternalFormat);

			
			
			virtual void prepare(int unit, TextureSampler *ts);
			virtual void restore(int unit);

			virtual int getNumberOfComponents(void);
			virtual int getDimension(void);
			virtual int getInternalFormat (void);
			virtual int getType (void);
			virtual int getFormat(void);
			virtual int getElementSize();
			virtual bool getMipmap();

			//virtual void prepare(int unit,IState *state);
			//virtual void restore(int unit,curitiba::render::IState *state);
			//virtual void bind (void);
			//virtual void unbind (void);
			//void enableCompareToTexture (void);
			//void disableCompareToTexture (void);
			//void enableObjectSpaceCoordGen (void);
			//void generateObjectSpaceCoords (TextureCoord aCoord, float *plane);


		protected:

			GLTexture (std::string label, std::string internalFormat,
				std::string aFormat, std::string aType, int width, int height, 
				void* data = NULL, bool mipmap = true );

			GLTexture (std::string label);

			void setData(std::string internalFormat, std::string aFormat, 
				std::string aType, int width, int height, unsigned char * data = NULL);

			static int getIndex(std::string StringArray[], int IntArray[], std::string aString);
			GLenum translateCoord (TextureCoord aCoord);

			int m_Dimension;
			int m_Format;
			int m_InternalFormat;
			int m_Type;
			bool m_Mipmap;

			//GLenum translateDimension (TextureDimension aDimension);
			//void setData(std::string internalFormat, std::string aFormat, 
			//	std::string aType, int width, int height);
			//GLTexture (std::string label, std::string internalFormat,
			//	std::string aFormat, std::string aType, int width, int height);
			//GLenum translateFormat (TextureFormat aFormat);
			//GLenum translateInternalFormat (TextureFormat aFormat);
			//GLenum translateType (TextureType aType);
			//GLenum translateTexWrapMode (TextureSampler::TextureWrapMode aMode);
			//GLenum translateTexEnvMode (TextureSampler::TextureEnvMode aMode);
			//GLenum translateTexCompareMode (TextureSampler::TextureCompareMode aMode);
			//GLenum translateTexMagMode (TextureSampler::TextureMagMode aMode);
			//GLenum translateTexMinMode (TextureSampler::TextureMinMode aMode);
			//GLenum translateTexCompareFunc (TextureSampler::TextureCompareFunc aFunc);
			//GLTexture(void) {}; 


		};
	};
};
#endif
