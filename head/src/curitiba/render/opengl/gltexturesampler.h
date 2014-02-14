#ifndef GLTEXTURESAMPLER_H
#define GLTEXTURESAMPLER_H



#include <curitiba/material/texturesampler.h>
#include <map>

#include <GL/glew.h>


using namespace curitiba::material;
using namespace curitiba::render;

namespace curitiba
{
	namespace render
	{
		class GLTextureSampler : public curitiba::material::TextureSampler
		{
		public:
			~GLTextureSampler(void);
			GLTextureSampler(Texture *t);

			void update();

			virtual void setTexProp(TextureEnumProp prop, int value);
			virtual void setTexProp(TextureFloat4Prop prop, float x, float y, float z, float w);
			virtual void setTexProp(TextureFloat4Prop prop, vec4& value);
			virtual int getTexProp(TextureEnumProp prop);
			virtual vec4* getTexProp(TextureFloat4Prop prop);

			virtual void prepare(int aUnit, int aDim);


			static std::map<int,int> TransTexWrapMode;
			static std::map<int,int> TransTexCompareFunc;
			static std::map<int,int> TransTexEnum;
			static std::map<int,int> TransTexMagMode;
			static std::map<int,int> TransTexCompareMode;
			static std::map<int,int> TransTexMinMode;
			static bool initMaps();

			//virtual void setMipmap(bool m);
			//static unsigned int TranslateTexWrapMode (GLTextureSampler::TextureWrapMode aMode);
			//static unsigned int TranslateTexCompareMode (GLTextureSampler::TextureCompareMode aMode);
			//static unsigned int TranslateTexMagMode (GLTextureSampler::TextureMagMode aMode);
			//static unsigned int TranslateTexMinMode (GLTextureSampler::TextureMinMode aMode);
			//static unsigned int TranslateTexCompareFunc (GLTextureSampler::TextureCompareFunc aFunc);
			//static unsigned int TranslateTexEnum(GLTextureSampler::TextureEnumProp p);
		protected:
			GLTextureSampler(void);
			static bool init;
		};
	};
};


#endif
