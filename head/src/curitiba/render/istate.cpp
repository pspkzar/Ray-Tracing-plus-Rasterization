#include <curitiba/render/istate.h>
#include <curitiba/config.h>

#ifdef CURITIBA_OPENGL
#include <curitiba/render/opengl/glstate.h>	
#elif CURITIBA_DIRECTX
#include <curitiba/render/dx/dxstate.h>
#endif

using namespace curitiba::render;

IState*
IState::create() {

#ifdef CURITIBA_OPENGL
	return new GlState ;
#elif CURITIBA_DIRECTX
	return new DXState;
#endif
}

IState::IState():
		//m_IntProps(COUNT_STATEPROPINT),
		//m_EnableProps(COUNT_STATESETTING),
		//m_FloatProps(COUNT_STATEPROPFLOAT),
		//m_Float4Props(COUNT_STATEPROPFLOAT4),
		//m_Bool4Props(COUNT_STATEPROPBOOL4),
		//m_EnumProps(COUNT_STATEPROPENUM),
		m_defColor(0.0f, 0.0f, 0.0f, 1.0f)
{

	//// Texture State
	//m_TexEnumProps.resize(COUNT_TEXTUREUNIT);
	//for(int u = TEXTURE0; u < COUNT_TEXTUREUNIT; u++) {
	//	
	//	m_TexEnumProps[u].resize(COUNT_TEXTUREENUMPROPS);
	//}
	//m_TexFloat4Props.resize(COUNT_TEXTUREUNIT);
	//for (int u = 0; u < COUNT_TEXTUREUNIT; u++) {
	//	
	//	m_TexFloat4Props[u].resize(COUNT_TEXTUREFLOAT4PROPS);
	//}

	//setDefault();
}


			
IState* 
IState::clone() {

	IState *res = IState::create();

	std::map<unsigned int, int>::iterator iterInt;
	iterInt = m_MapIntProps.begin();
	for ( ; iterInt != m_MapIntProps.end(); ++iterInt) {
	
		res->m_MapIntProps[iterInt->first] = iterInt->second;
	}

	std::map<unsigned int, bool>::iterator iterBool;
	iterBool = m_MapEnableProps.begin();
	for ( ; iterBool != m_MapEnableProps.end(); ++iterBool) {
	
		res->m_MapEnableProps[iterBool->first] = iterBool->second;
	}

	std::map<unsigned int, float>::iterator iterFloat;
	iterFloat = m_MapFloatProps.begin();
	for ( ; iterFloat != m_MapFloatProps.end(); ++iterFloat) {
	
		res->m_MapFloatProps[iterFloat->first] = iterFloat->second;
	}

	std::map<unsigned int, vec4>::iterator iterVec4;
	iterVec4 = m_MapFloat4Props.begin();
	for ( ; iterVec4 != m_MapFloat4Props.end(); ++iterVec4) {
	
		res->m_MapFloat4Props[iterVec4->first] = iterVec4->second;
	}

	std::map<unsigned int, BoolArray>::iterator iterBool4;
	iterBool4 = m_MapBool4Props.begin();
	for ( ; iterBool4 != m_MapBool4Props.end(); ++iterBool4) {
	
		res->m_MapBool4Props[iterBool4->first] = iterBool4->second;
	}

	iterInt = m_MapEnumProps.begin();
	for ( ; iterInt != m_MapEnumProps.end(); ++iterInt) {
	
		res->m_MapEnumProps[iterInt->first] = iterInt->second;
	}


	//for (int i = 0; i < IState::COUNT_STATEPROPINT; ++i) 
	//	res->m_IntProps[i] = m_IntProps[i];

	//for (int i = 0; i < IState::COUNT_STATEPROPFLOAT; ++i) 
	//	res->m_FloatProps[i] = m_FloatProps[i];

	//for (int i = 0; i < IState::COUNT_STATESETTING; ++i) 
	//	res->m_EnableProps[i] = m_EnableProps[i];

	//for (int i = 0; i < IState::COUNT_STATEPROPFLOAT4; ++i) 
	//	res->m_Float4Props[i].set(&m_Float4Props[i]);

	//for (int i = 0; i < IState::COUNT_STATEPROPBOOL4; ++i) {
	//	res->m_Bool4Props[i].set(&m_Bool4Props[i]);
	//}

	//for (int i = 0; i < IState::COUNT_STATEPROPENUM; ++i) 
	//	res->m_EnumProps[i] = m_EnumProps[i];

	//for(int i = TEXTURE0 ; i < COUNT_TEXTUREUNIT ; ++i) {
	//
	//	for(int prop = 0; prop < COUNT_TEXTUREENUMPROPS; ++prop) {
	//		res->m_TexEnumProps[i][prop] = m_TexEnumProps[i][prop];
	//	}
	//	for(int prop = 0; prop < COUNT_TEXTUREFLOAT4PROPS; ++prop) {
	//		res->m_TexFloat4Props[i][prop] = m_TexFloat4Props[i][prop];
	//	}
	//}


	return res;
}


void 
IState::clear() {

	m_MapIntProps.clear();
	m_MapEnableProps.clear();
	m_MapFloatProps.clear();
	m_MapFloat4Props.clear();
	m_MapBool4Props.clear();
	m_MapEnumProps.clear();
}


void 
IState::setDefault() {

	m_Name = "default";

	// ORDERING
	m_MapIntProps[ORDER] = 0;
	m_MapEnumProps[ORDER_TYPE] = FRONT_TO_BACK;


	// FOG
	m_MapEnableProps[FOG] = false;
	m_MapFloat4Props[FOG_COLOR].set(0.0f, 0.0f, 0.0f, 1.0f);
	m_MapEnumProps[FOG_MODE] = EXP;
	m_MapFloatProps[FOG_DENSITY] = 1.0f;
	m_MapFloatProps[FOG_START] = 0.0f;
	m_MapFloatProps[FOG_END] = 1.0f;
	m_MapEnumProps[FOG_COORD_SRC] = FRAGMENT_DEPTH;

	
	// ALPHA TEST
	m_MapEnableProps[ALPHA_TEST] = false;
	m_MapEnumProps[ALPHA_FUNC] = ALWAYS;
	m_MapFloatProps[ALPHA_VALUE] = 0.0f;


	// BLEND
	m_MapEnableProps[BLEND] = false;
	m_MapFloat4Props[BLEND_COLOR].set(0.0f, 0.0f, 0.0f, 1.0f);
	m_MapEnumProps[BLEND_EQUATION] = ADD;
	m_MapEnumProps[BLEND_SRC] = ONE;
	m_MapEnumProps[BLEND_DST] = ZERO;

	// DEPTH TEST
	m_MapEnableProps[DEPTH_TEST] = true;
	m_MapEnumProps[DEPTH_FUNC] = LESS;
	m_MapEnableProps[DEPTH_MASK] = true;


	// CULLING
	m_MapEnableProps[CULL_FACE] = true;
	m_MapEnumProps[CULL_TYPE] = BACK_FACE;


	// COLOR MASK
	m_MapEnableProps[COLOR_MASK] = false;
	m_MapBool4Props[COLOR_MASK_B4].set(true,true,true,true);

	//m_EnableProps[COLOR_MASK] = false;
	//m_Bool4Props[COLOR_MASK_B4].set(true,true,true,true);
	//m_IntProps[ORDER] = 0;
	//m_EnumProps[ORDER_TYPE] = FRONT_TO_BACK;
	//m_EnableProps[FOG] = false;
	//m_Float4Props[FOG_COLOR].set(0.0f, 0.0f, 0.0f, 1.0f);
	//m_EnumProps[FOG_MODE] = EXP;
	//m_FloatProps[FOG_DENSITY] = 1.0f;
	//m_FloatProps[FOG_START] = 0.0f;
	//m_FloatProps[FOG_END] = 1.0f;
	//m_EnumProps[FOG_COORD_SRC] = FRAGMENT_DEPTH;
	//m_EnableProps[ALPHA_TEST] = false;
	//m_EnumProps[ALPHA_FUNC] = ALWAYS;
	//m_FloatProps[ALPHA_VALUE] = 0.0f;
	//m_EnableProps[BLEND] = false;
	//m_Float4Props[BLEND_COLOR].set(0.0f, 0.0f, 0.0f, 1.0f);
	//m_EnumProps[BLEND_EQUATION] = ADD;
	//m_EnumProps[BLEND_SRC] = ONE;
	//m_EnumProps[BLEND_DST] = ZERO;
	//m_EnableProps[DEPTH_TEST] = true;
	//m_EnumProps[DEPTH_FUNC] = LESS;
	//m_EnableProps[DEPTH_MASK] = true;
	//m_EnableProps[CULL_FACE] = true;
	//m_EnumProps[CULL_TYPE] = BACK_FACE;

	// Texture States

	//for(int i = TEXTURE0 ; i < COUNT_TEXTUREUNIT ; i++) {
	//
	//	for(int prop = 0; prop < COUNT_TEXTUREENUMPROPS; prop++) {
	//		m_TexEnumProps[i][prop] = 0;
	//	}
	//	for(int prop = 0; prop < COUNT_TEXTUREFLOAT4PROPS; prop++) {
	//		m_TexFloat4Props[i][prop].set(1.0f, 1.0f, 1.0f, 1.0f);
	//	}
	//}
}

//
// GET methods
//

std::string
IState::getName() {

	return(m_Name);
}


int 
IState::getPropi(StatePropInt aProp) {

	//return(m_IntProps[aProp]);

	if (m_MapIntProps.count(aProp) != 0)
		return (m_MapIntProps[aProp]);

	//else return default value
	switch(aProp) {
		case ORDER: return 0;
		default: return 0;
	}
}


int 
IState::getPrope(StatePropEnum aProp){

	//return(m_EnumProps[aProp]);

	if (m_MapEnumProps.count(aProp) != 0)
		return (m_MapEnumProps[aProp]);

	//else return default value
	switch(aProp) {
		case FOG_MODE: return EXP;
		case FOG_COORD_SRC: return FRAGMENT_DEPTH;
		case DEPTH_FUNC: return LESS;
		case CULL_TYPE: return BACK_FACE;
		case ORDER_TYPE: return FRONT_TO_BACK;
		case BLEND_SRC: return ONE;
		case BLEND_DST: return ZERO;
		case BLEND_EQUATION: return ADD;
		case ALPHA_FUNC: return ALWAYS;
		default: return 0;
	}
}


float 
IState::getPropf(StatePropFloat aProp) {
	
	//return(m_FloatProps[aProp]);

	if (m_MapFloatProps.count(aProp) != 0)
		return (m_MapFloatProps[aProp]);

	//else return default value
	switch(aProp) {
		case FOG_START: return 0.0f;
		case FOG_END: return 1.0f;
		case FOG_DENSITY: return 1.0f;
		case ALPHA_VALUE: return 0.0f;
		default: return 0.0f;
	}
}


const vec4&
IState::getProp4f(StatePropFloat4 aProp) {

	//return m_Float4Props[aProp];

	if (m_MapFloat4Props.count(aProp) != 0)
		return (m_MapFloat4Props[aProp]);

	//else return default value
	switch(aProp) {
		case BLEND_COLOR: return m_defColor;
		case FOG_COLOR: return m_defColor;
		default: return m_defColor;
	}
}


bool *
IState::getProp4b(StatePropBool4 aProp) {

	bool *a = (bool *)malloc(4 * sizeof(bool));

	if (m_MapBool4Props.count(aProp) != 0) {
		
		a[0] = m_MapBool4Props[aProp].values[0];
		a[1] = m_MapBool4Props[aProp].values[1];
		a[2] = m_MapBool4Props[aProp].values[2];
		a[3] = m_MapBool4Props[aProp].values[3];
	}
	else {
		a[0] = true;
		a[1] = true;
		a[2] = true;
		a[3] = true;
	}
	return a;
}


bool
IState::getSetting(StateSetting aProp) {

	//return(m_EnableProps[s]);

	if (m_MapEnableProps.count(aProp) != 0)
		return (m_MapEnableProps[aProp]);

	//else return default value
	switch(aProp) {
		case BLEND: return false;
		case FOG: return false;
		case ALPHA_TEST: return false;
		case DEPTH_TEST: return true;
		case DEPTH_MASK: return true;
		case CULL_FACE: return true;
		case COLOR_MASK: return false;
		default: return true;
	}
}

//
// SET methods
//

void 
IState::setName(std::string aName) {

	m_Name = aName;
}


void 
IState::enable(StateSetting s, bool value) {

//	m_EnableProps[s] = value;
	m_MapEnableProps[s] = value;
}


void
IState::setProp(StatePropInt prop, int value) {

//	m_IntProps[prop] = value;
	m_MapIntProps[prop] = value;
}


void
IState::setProp(StatePropFloat prop, float value) {

//	m_FloatProps[prop] = value;
	m_MapFloatProps[prop] = value;
}


void
IState::setProp(StatePropFloat4 prop, float r, float g, float b, float a){

//	m_Float4Props[prop].set(r,g,b,a);
	m_MapFloat4Props[prop].set(r,g,b,a);
}


void
IState::setProp(StatePropFloat4 prop, const vec4& values){

//	m_Float4Props[prop].set(values);
	m_MapFloat4Props[prop].set(values);

}


void
IState::setProp(StatePropBool4 prop, bool r, bool g, bool b, bool a){

//	m_Bool4Props[prop].set(r,g,b,a);
	m_MapBool4Props[prop].set(r,g,b,a);
}


void
IState::setProp(StatePropBool4 prop, bool* values) {

//	m_Bool4Props[prop].set(values[0], values [1], values [2], values[3]);
	m_MapBool4Props[prop].set(values[0], values [1], values [2], values[3]);
}


void
IState::setProp(StatePropEnum prop, int value){

//	m_EnumProps[prop] = value;
	m_MapEnumProps[prop] = value;
}



/* Is a property defined? */

bool 
IState::isSetProp(StatePropInt prop) {

	return (m_MapIntProps.count(prop) != 0);
}


bool 
IState::isSetProp(StatePropFloat prop) {

	return (m_MapFloatProps.count(prop) != 0);
}


bool 
IState::isSetProp(StatePropFloat4 prop) {

	return (m_MapFloat4Props.count(prop) != 0);
}


bool 
IState::isSetProp(StatePropBool4 prop) {

	return (m_MapBool4Props.count(prop) != 0);
}


bool 
IState::isSetProp(StatePropEnum prop) {

	return (m_MapEnumProps.count(prop) != 0);
}





// ---------------------------------------------------------------------
//
//								TRANSLATE STRINGS
//
// ---------------------------------------------------------------------


int 
IState::translateStringToOrderTypeEnum(std::string s) {
	
	if (s == "FRONT_TO_BACK")
		return(FRONT_TO_BACK);
	if (s == "BACK_TO_FRONT")
		return(BACK_TO_FRONT);
	if (s == "NONE")
		return(NONE);
	return(-1);
}

int 
IState::translateStringToCullTypeEnum(std::string s) {

	if (s == "FRONT_FACE")
		return(FRONT_FACE);
	if (s == "BACK_FACE")
		return(BACK_FACE);
	if (s == "FRONT_AND_BACK_FACE")
		return(FRONT_AND_BACK_FACE);
	return(-1);
}

int 
IState::translateStringToBlendFuncEnum(std::string s) {

	if (s == "ZERO")
		return(ZERO);
	if (s == "ONE")
		return(ONE);
	if (s == "SRC_COLOR")
		return(SRC_COLOR);
	if (s == "ONE_MINUS_SRC_COLOR")
		return(ONE_MINUS_SRC_COLOR);
	if (s == "DST_COLOR")
		return(DST_COLOR);
	if (s == "ONE_MINUS_DST_COLOR")
		return(ONE_MINUS_DST_COLOR);
	if (s == "SRC_ALPHA")
		return(SRC_ALPHA);
	if (s == "ONE_MINUS_SRC_ALPHA")
		return(ONE_MINUS_SRC_ALPHA);
	if (s == "DST_ALPHA")
		return(DST_ALPHA);
	if (s == "ONE_MINUS_DST_ALPHA")
		return(ONE_MINUS_DST_ALPHA);
	if (s == "SRC_ALPHA_SATURATE")
		return(SRC_ALPHA_SATURATE);
	if (s == "CONSTANT_COLOR")
		return(CONSTANT_COLOR);
	if (s == "ONE_MINUS_CONSTANT_COLOR")
		return(ONE_MINUS_CONSTANT_COLOR);
	if (s == "CONSTANT_ALPHA")
		return(CONSTANT_ALPHA);
	if (s == "ONE_MINUS_CONSTANT_ALPHA")
		return(ONE_MINUS_CONSTANT_ALPHA);
	return(-1);
}

int 
IState::translateStringToBlendEquationEnum(std::string s) {

	if (s == "ADD")
		return(ADD);
	if (s == "SUBTRACT")
		return(SUBTRACT);
	if (s == "REVERSE_SUBRACT")
		return(REVERSE_SUBTRACT);
	if (s == "MIN")
		return(MIN);
	if (s == "MAX")
		return(MAX);
	return(-1);
}

int 
IState::translateStringToFogModeEnum(std::string s) {

	if (s == "LINEAR")
		return(LINEAR);
	if (s == "EXP")
		return(EXP);
	if (s == "EXP2")
		return(EXP2);
	return(-1);
}

int 
IState::translateStringToFogCoordSrcEnum(std::string s) {

	if (s == "FOG_COORD")
		return(FOG_COORD);
	if (s == "FRAGMENT_DEPTH")
		return(FRAGMENT_DEPTH);
	return(-1);
}

int 
IState::translateStringToFuncEnum(std::string s) {

	if (s == "NEVER")
		return(NEVER);
	if (s == "ALWAYS")
		return(ALWAYS);
	if (s == "LESS")
		return(LESS);
	if (s == "LEQUAL")
		return(LEQUAL);
	if (s == "GEQUAL")
		return(GEQUAL);
	if (s == "GREATER")
		return(GREATER);
	if (s == "EQUAL")
		return(EQUAL);
	if (s == "NOT_EQUAL")
		return(NOT_EQUAL);

	return(-1);
}

//int 
//IState::translateStringToTexEnvModeEnum(std::string s) {
//
//	if (s == "MODULATE")
//		return(TEXTURE_ENV_MODULATE);
//	if (s == "DECAL")
//		return(TEXTURE_ENV_DECAL);
//	if (s == "BLEND")
//		return(TEXTURE_ENV_BLEND);
//	if (s == "REPLACE")
//		return(TEXTURE_ENV_REPLACE);
//	return(-1);
//}
//
//int 
//IState::translateStringToTexWrapModeEnum(std::string s) {
//
//	if (s == "CLAMP_TO_EDGE")
//		return(TEXTURE_WRAP_CLAMP_TO_EDGE);
//	if (s == "CLAMP_TO_BORDER")
//		return(TEXTURE_WRAP_CLAMP_TO_BORDER);
//	if (s == "REPEAT")
//		return(TEXTURE_WRAP_REPEAT);
//	if (s == "MIRRORED_REPEAT")
//		return(TEXTURE_WRAP_MIRRORED_REPEAT);
//	return(-1);
//}
//
//int 
//IState::translateStringToTexMinModeEnum(std::string s) {
//
//	if (s == "NEAREST")
//		return(TEXTURE_MIN_NEAREST);
//	if (s == "LINEAR")
//		return(TEXTURE_MIN_LINEAR);
//	if (s == "NEAREST_MIPMAP_NEAREST")
//		return(TEXTURE_MIN_NEAREST_MIPMAP_NEAREST);
//	if (s == "NEAREST_MIPMAP_LINEAR")
//		return(TEXTURE_MIN_NEAREST_MIPMAP_LINEAR);
//	if (s == "LINEAR_MIPMAP_NEAREST")
//		return(TEXTURE_MIN_LINEAR_MIPMAP_NEAREST);
//	if (s == "LINEAR_MIPMAP_LINEAR")
//		return(TEXTURE_MIN_LINEAR_MIPMAP_LINEAR);
//	return(-1);
//}
//
//int 
//IState::translateStringToTexMagModeEnum(std::string s) {
//
//	if (s == "NEAREST")
//		return(TEXTURE_MAG_NEAREST);
//	if (s == "LINEAR")
//		return(TEXTURE_MAG_LINEAR);
//	return(-1);
//}
//
//int 
//IState::translateStringToTexCompareFuncEnum(std::string s) {
//
//	if (s == "NONE")
//		return(COMPARE_NONE);
//	if (s == "LEQUAL")
//		return(COMPARE_LEQUAL);
//	if (s == "GEQUAL")
//		return(COMPARE_GEQUAL);
//	if (s == "LESS")
//		return(COMPARE_LESS);
//	if (s == "GREATER")
//		return(COMPARE_GREATER);
//	if (s == "EQUAL")
//		return(COMPARE_EQUAL);
//	if (s == "NOTEQUAL")
//		return(COMPARE_NOTEQUAL);
//	if (s == "ALWAYS")
//		return(COMPARE_ALWAYS);
//	if (s == "NEVER")
//		return(COMPARE_NEVER);
//
//	return(-1);
//}



//int 
//IState::translateStringToTexCompareModeEnum(std::string s) {
//
//	if (s == "NONE")
//		return(TEXTURE_COMPARE_NONE);
//	if (s == "COMPARE_REF_TO_TEXTURE")
//		return(TEXTURE_COMPARE_REF_TO_TEXTURE);
//
//	return(-1);
//}



// ---------------------------------------------------------------------
//
//								TEXTURE STATES
//
// ---------------------------------------------------------------------


//void 
//IState::setTexProp(TextureUnit i, TextureEnumProp prop, int value) {
//
//	m_TexEnumProps[i][prop] = value;
//}
//
//
//void 
//IState::setTexProp(TextureUnit i, TextureFloat4Prop prop, float x, float y, float z, float w) {
//
//	m_TexFloat4Props[i][prop].set(x,y,z,w);
//}
//			
//
//void 
//IState::setTexProp(TextureUnit i, TextureFloat4Prop prop, vec4& value) {
//
//	m_TexFloat4Props[i][prop].set(value.x,value.y,value.z,value.w);
//}
//			
//
//int 
//IState::getTexProp(TextureUnit i, TextureEnumProp prop) {
//
//	return(m_TexEnumProps[i][prop]);
//}
//			
//
//vec4* 
//IState::getTexProp(TextureUnit i, TextureFloat4Prop prop) {
//
//	return(&m_TexFloat4Props[i][prop]);
//}
