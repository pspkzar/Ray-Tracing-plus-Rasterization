#ifndef __TEXTURE_SAMPLER__
#define __TEXTURE_SAMPLER__

#include <vector>

#include <curitiba/math/vec4.h>
#include <curitiba/render/texture.h>

using namespace curitiba::math;

namespace curitiba {

	namespace render {
		class Texture;
	}
}

namespace curitiba {

	namespace material {
	
		class TextureSampler {
		
		public:

			typedef enum {TEXTURE_WRAP_S, TEXTURE_WRAP_T, TEXTURE_WRAP_R, 
					TEXTURE_MIN_FILTER, TEXTURE_MAG_FILTER, 
					TEXTURE_COMPARE_FUNC, TEXTURE_COMPARE_MODE, COUNT_TEXTUREENUMPROPS} TextureEnumProp;
			typedef enum {TEXTURE_BORDER_COLOR, COUNT_TEXTUREFLOAT4PROPS} TextureFloat4Prop;

			typedef enum {TEXTURE_COMPARE_NONE, TEXTURE_COMPARE_REF_TO_TEXTURE} TextureCompareMode;

			typedef enum {TEXTURE_WRAP_REPEAT,TEXTURE_WRAP_CLAMP_TO_EDGE, 
					TEXTURE_WRAP_CLAMP_TO_BORDER, TEXTURE_WRAP_MIRRORED_REPEAT} TextureWrapMode;

			typedef enum {TEXTURE_MIN_NEAREST, TEXTURE_MIN_LINEAR, 
					TEXTURE_MIN_NEAREST_MIPMAP_NEAREST,
					TEXTURE_MIN_NEAREST_MIPMAP_LINEAR,
					TEXTURE_MIN_LINEAR_MIPMAP_NEAREST,
					TEXTURE_MIN_LINEAR_MIPMAP_LINEAR} TextureMinMode;

			typedef enum {TEXTURE_MAG_NEAREST, TEXTURE_MAG_LINEAR} TextureMagMode;

			typedef enum {COMPARE_NONE, COMPARE_LEQUAL, COMPARE_GEQUAL, COMPARE_LESS,
					COMPARE_GREATER, COMPARE_EQUAL, COMPARE_NOTEQUAL,
					COMPARE_ALWAYS, COMPARE_NEVER} TextureCompareFunc;


			static TextureSampler* create(curitiba::render::Texture *t);

			virtual unsigned int getSamplerID();

			virtual void prepare(int aUnit, int aDim) = 0;
			virtual void setTexProp(TextureEnumProp prop, int value) = 0;
			virtual void setTexProp(TextureFloat4Prop prop, float x, float y, float z, float w) = 0;
			virtual void setTexProp(TextureFloat4Prop prop, vec4& value) = 0;
			virtual int getTexProp(TextureEnumProp prop) = 0;
			virtual vec4* getTexProp(TextureFloat4Prop prop) = 0;

			static int translateStringToTexWrapModeEnum(std::string s);
			static int translateStringToTexMinModeEnum(std::string s);
			static int translateStringToTexMagModeEnum(std::string s);
			static int translateStringToTexCompareFuncEnum(std::string s);
			static int translateStringToTexCompareModeEnum(std::string s);
			static int translateStringToTexEnvModeEnum(std::string s);

			TextureSampler() ;		
		
		protected:

			//virtual void update() = 0;

			unsigned int m_SamplerID;
			bool m_Mipmap;
			std::vector<int> m_TexEnumProps;
			std::vector<vec4> m_TexFloat4Props;

			vec4 m_defColor;
		
		};
	};
};


#endif