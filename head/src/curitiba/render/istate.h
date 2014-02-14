#ifndef __STATE__
#define __STATE__


#include <string>
#include <vector>
#include <map>
#include <curitiba/math/vec4.h>

using namespace curitiba::math;


class BoolArray {

public:

	bool values[4];

	BoolArray() {
		values[0] = false;
		values[1] = false;
		values[2] = false;
		values[3] = false;
	}

	void set(BoolArray *aBA) {
		values[0] = aBA->values[0];
		values[1] = aBA->values[1];
		values[2] = aBA->values[2];
		values[3] = aBA->values[3];
	}

	void set(bool x, bool y, bool z, bool w) {
	
		values[0] = x;
		values[1] = y;
		values[2] = z;
		values[3] = w;
	}

	void set(bool *b) {
	
		values[0] = b[0];
		values[1] = b[1];
		values[2] = b[2];
		values[3] = b[3];
	}

	bool operator == (const BoolArray &m) {

		if (this != &m) {

			if (values[0] != m.values[0] || values[1] != m.values[1] ||
				values[2] != m.values[2] || values[3] != m.values[3] )
				return false;
		}
		return true;
	}
	bool operator != (const BoolArray &m) {

		if (this != &m) {

			if (values[0] != m.values[0] || values[1] != m.values[1] ||
				values[2] != m.values[2] || values[3] != m.values[3] )
				return true;
		}
		return false;
	}
};

namespace curitiba
{
	namespace render
	{
		class IState {

		public:

			//-----------------------------------------------------------------
			// These enums can grow safely as long as
			// the new properties are appended at the end, but before the 
			// COUNT_* item
			// For enums that do not contain the COUNT_* item, always
			// append in the end
			//-----------------------------------------------------------------
			typedef enum {BOOL,ENUM,INT,FLOAT,FLOAT4, BOOL4,COUNT_VARTYPE} VarType;

			typedef enum  {BLEND,FOG,ALPHA_TEST,DEPTH_TEST,
				CULL_FACE,COLOR_MASK, DEPTH_MASK, COUNT_STATESETTING} StateSetting; 

			typedef enum {FOG_MODE,FOG_COORD_SRC,
					DEPTH_FUNC, CULL_TYPE, ORDER_TYPE, 
					BLEND_SRC, BLEND_DST, BLEND_EQUATION,ALPHA_FUNC,COUNT_STATEPROPENUM} StatePropEnum;

			typedef enum {FOG_START, FOG_END, FOG_DENSITY, ALPHA_VALUE,COUNT_STATEPROPFLOAT} StatePropFloat;

			typedef enum {ORDER,COUNT_STATEPROPINT} StatePropInt;

			typedef enum StatePropFloat4  {BLEND_COLOR, FOG_COLOR,COUNT_STATEPROPFLOAT4} StatePropFloat4;

			typedef enum {COLOR_MASK_B4, COUNT_STATEPROPBOOL4} StatePropBool4;

			// All the following enums must be present in the 
			// translateStringToEnum
			typedef enum  { FRONT_TO_BACK, BACK_TO_FRONT, NONE} OrderType;
			typedef enum  {FRONT_FACE, BACK_FACE, FRONT_AND_BACK_FACE} CullType;

			typedef enum {LESS, NEVER,ALWAYS,LEQUAL,
				EQUAL, GEQUAL, GREATER, NOT_EQUAL} Func;

			typedef enum  {ZERO,ONE,SRC_COLOR,ONE_MINUS_SRC_COLOR,DST_COLOR,
				ONE_MINUS_DST_COLOR, SRC_ALPHA, ONE_MINUS_SRC_ALPHA, DST_ALPHA,
				ONE_MINUS_DST_ALPHA, SRC_ALPHA_SATURATE, CONSTANT_COLOR, 
				ONE_MINUS_CONSTANT_COLOR, CONSTANT_ALPHA, ONE_MINUS_CONSTANT_ALPHA} BlendFunc;
			typedef enum  {ADD,SUBTRACT,REVERSE_SUBTRACT,MIN,MAX} BlendEquation;

			typedef enum {LINEAR, EXP, EXP2} FogMode;
			typedef enum {FOG_COORD, FRAGMENT_DEPTH} FogCoordSrc;

			virtual ~IState(void) {};

			static IState* create();
			virtual void setDefault();

			virtual void setName(std::string name);
			virtual std::string getName();

			virtual IState* clone();
			virtual void clear();

			virtual void set() = 0;
			virtual void setDiff(IState *defState, IState *pState) = 0;

			virtual void enable(StateSetting ss, bool value);
			virtual bool getSetting(StateSetting ss);

			virtual void setProp(StatePropInt prop, int value);
			virtual void setProp(StatePropFloat prop, float value);
			virtual void setProp(StatePropFloat4 prop, float r, float g, float b, float a);
			//virtual void setProp(StatePropFloat4 prop, vec4* color);
			virtual void setProp(StatePropFloat4 prop, const vec4& color);
			virtual void setProp(StatePropBool4 prop, bool r, bool g, bool b, bool a);
			virtual void setProp(StatePropBool4 prop, bool* values); 
			virtual void setProp(StatePropEnum prop, int value);

			virtual bool isSetProp(StatePropInt prop);
			virtual bool isSetProp(StatePropFloat prop);
			virtual bool isSetProp(StatePropFloat4 prop);
			virtual bool isSetProp(StatePropBool4 prop);
			virtual bool isSetProp(StatePropEnum prop);

			virtual int getPropi(StatePropInt prop);
			virtual float getPropf(StatePropFloat prop);
			virtual const vec4& getProp4f(StatePropFloat4 prop);
			virtual bool* getProp4b(StatePropBool4 prop);
			virtual int getPrope(StatePropEnum prop);

			static int translateStringToOrderTypeEnum(std::string s);
			static int translateStringToCullTypeEnum(std::string s);
			static int translateStringToFuncEnum(std::string s);
			static int translateStringToBlendFuncEnum(std::string s);
			static int translateStringToBlendEquationEnum(std::string s);
			static int translateStringToFogModeEnum(std::string s);
			static int translateStringToFogCoordSrcEnum(std::string s);

			// The following enums are not saved in CBO files
			// these can only be found in materials present on mlib files
			//typedef enum {TEXTURE0, TEXTURE1, TEXTURE2, TEXTURE3, TEXTURE4, TEXTURE5,
			//		TEXTURE6,TEXTURE7,COUNT_TEXTUREUNIT} TextureUnit;

			//typedef enum {TEXTURE_WRAP_S, TEXTURE_WRAP_T, TEXTURE_WRAP_R, 
			//		TEXTURE_MIN_FILTER, TEXTURE_MAG_FILTER, 
			//		TEXTURE_COMPARE_FUNC, TEXTURE_COMPARE_MODE,TEXTURE_ENV_MODE,COUNT_TEXTUREENUMPROPS} TextureEnumProp;
			//typedef enum {TEXTURE_BORDER_COLOR, COUNT_TEXTUREFLOAT4PROPS} TextureFloat4Prop;

			//typedef enum {TEXTURE_ENV_MODULATE,TEXTURE_ENV_DECAL,
			//		TEXTURE_ENV_BLEND, TEXTURE_ENV_REPLACE} TextureEnvMode;

			//typedef enum {TEXTURE_COMPARE_NONE, TEXTURE_COMPARE_REF_TO_TEXTURE} TextureCompareMode;

			//typedef enum {TEXTURE_WRAP_REPEAT,TEXTURE_WRAP_CLAMP_TO_EDGE, 
			//		TEXTURE_WRAP_CLAMP_TO_BORDER, TEXTURE_WRAP_MIRRORED_REPEAT} TextureWrapMode;

			//typedef enum {TEXTURE_MIN_NEAREST, TEXTURE_MIN_LINEAR, 
			//		TEXTURE_MIN_NEAREST_MIPMAP_NEAREST,
			//		TEXTURE_MIN_NEAREST_MIPMAP_LINEAR,
			//		TEXTURE_MIN_LINEAR_MIPMAP_NEAREST,
			//		TEXTURE_MIN_LINEAR_MIPMAP_LINEAR} TextureMinMode;

			//typedef enum {TEXTURE_MAG_NEAREST, TEXTURE_MAG_LINEAR} TextureMagMode;

			//typedef enum {COMPARE_NONE, COMPARE_LEQUAL, COMPARE_GEQUAL, COMPARE_LESS,
			//		COMPARE_GREATER, COMPARE_EQUAL, COMPARE_NOTEQUAL,
			//		COMPARE_ALWAYS, COMPARE_NEVER} TextureCompareFunc;
		
			//static int translateStringToTexWrapModeEnum(std::string s);
			//static int translateStringToTexMinModeEnum(std::string s);
			//static int translateStringToTexMagModeEnum(std::string s);
			//static int translateStringToTexCompareFuncEnum(std::string s);
			//static int translateStringToTexCompareModeEnum(std::string s);
			//static int translateStringToTexEnvModeEnum(std::string s);
			//virtual void getCurrent() = 0;
			//SINCE A TEXTURE CAN BE USED IN DIFFERENT MATERIALS
			//WITH DIFFERENT SETTINGS THESE SHOULD BE USED WHENEVER A 
			//TEXTURE IS "PREPARED"
			//virtual void setTexProp(TextureUnit i, TextureEnumProp prop, int value);
			//virtual void setTexProp(TextureUnit i, TextureFloat4Prop prop, float x, float y, float z, float w);
			//virtual void setTexProp(TextureUnit i, TextureFloat4Prop prop, vec4& value);
			//virtual int getTexProp(TextureUnit i, TextureEnumProp prop);
			//virtual vec4* getTexProp(TextureUnit i, TextureFloat4Prop prop);

		protected:
			IState();

			std::string m_Name;
			vec4 m_defColor;


			std::map<unsigned int, int> m_MapIntProps;
			std::map<unsigned int, bool> m_MapEnableProps;
			std::map<unsigned int, float> m_MapFloatProps;
			std::map<unsigned int, vec4> m_MapFloat4Props;
			std::map<unsigned int, BoolArray> m_MapBool4Props;
			std::map<unsigned int, int> m_MapEnumProps;


			//static const int _i = COUNT_TEXTUREUNIT;
			//static const int _j = COUNT_TEXTUREENUMPROPS;
			//std::vector<std::vector <int>> m_TexEnumProps;
			//std::vector<std::vector <vec4>> m_TexFloat4Props;
			//std::vector<int> m_IntProps;
			//std::vector<bool> m_EnableProps;
			//std::vector<float> m_FloatProps;
			//std::vector<vec4> m_Float4Props;
			//std::vector<BoolArray> m_Bool4Props;
			//std::vector<int> m_EnumProps;
		};
	};
};

#endif
