#ifndef GLTEXTURE_CUBEMAP_H
#define GLTEXTURE_CUBEMAP_H

#include <curitiba/render/textureCubeMap.h>
#include <curitiba/scene/camera.h>

#include <curitiba.h>

#include <GL/glew.h>

using namespace curitiba::render;

namespace curitiba
{
	namespace render
	{
		class GLTextureCubeMap : public TextureCubeMap
		{
		friend class TextureCubeMap;

		public:

			~GLTextureCubeMap(void);

			virtual int getDimension(void);
			virtual int getInternalFormat (void);
			virtual int getType (void);
			virtual int getNumberOfComponents(void);
			virtual int getFormat(void);
			virtual int getElementSize(){return 0;};
			virtual bool getMipmap() {return (m_Mipmap);};

			virtual void prepare(int unit, curitiba::material::TextureSampler *ts);
			virtual void restore(int unit);

			//void enableCompareToTexture (void);
			//void disableCompareToTexture (void);
			//void enableObjectSpaceCoordGen (void);
			//void generateObjectSpaceCoords (TextureCoord aCoord, float *plane);
			//virtual void bind (void);
			//virtual void unbind (void);
			/*virtual void prepare(int unit,IState *state);
			virtual void restore(int unit);*/


		protected:
			GLTextureCubeMap (std::string label, std::vector<std::string> files, 
				std::string internalFormat,
				std::string aFormat, std::string aType, int width, unsigned char** data, bool mipmap = true );
		private:

			int getIndex(std::string StringArray[], int IntArray[], std::string aString);
			GLenum translateCoord (TextureCoord aCoord);

			static int faces[6];
			int m_Format;
			int m_InternalFormat;
			int m_Type;
			bool m_Mipmap;

//			GLTexture(void) {}; 
			//GLenum translateFormat (TextureFormat aFormat);
			//GLenum translateType (TextureType aType);
			//GLenum translateTexMagMode (TextureSampler::TextureMagMode aMode);
			//GLenum translateTexMinMode (TextureSampler::TextureMinMode aMode);
			//GLenum translateTexWrapMode (TextureSampler::TextureWrapMode aMode);
			//GLenum translateTexCompareMode (TextureSampler::TextureCompareMode aMode);
			//GLenum translateTexCompareFunc (TextureSampler::TextureCompareFunc aFunc);
		};
	};
};
#endif
