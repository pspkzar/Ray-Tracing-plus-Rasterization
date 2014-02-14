#include <curitiba/render/opengl/gltexturesampler.h>

//#include <GL/glew.h>

using namespace curitiba::render;

std::map<int,int> GLTextureSampler::TransTexMagMode;
std::map<int,int> GLTextureSampler::TransTexWrapMode; 
std::map<int,int> GLTextureSampler::TransTexEnum;
std::map<int,int> GLTextureSampler::TransTexMinMode;
std::map<int,int> GLTextureSampler::TransTexCompareMode;
std::map<int,int> GLTextureSampler::TransTexCompareFunc; 

bool GLTextureSampler::init = initMaps();

bool GLTextureSampler::initMaps() {

	TransTexEnum[TEXTURE_WRAP_S] = GL_TEXTURE_WRAP_S;
	TransTexEnum[TEXTURE_WRAP_T] = GL_TEXTURE_WRAP_T;
	TransTexEnum[TEXTURE_WRAP_R] = GL_TEXTURE_WRAP_R;
	TransTexEnum[TEXTURE_MIN_FILTER] = GL_TEXTURE_MIN_FILTER;
	TransTexEnum[TEXTURE_MAG_FILTER] = GL_TEXTURE_MAG_FILTER;
	TransTexEnum[TEXTURE_COMPARE_FUNC] = GL_TEXTURE_COMPARE_FUNC;
	TransTexEnum[TEXTURE_COMPARE_MODE] = GL_TEXTURE_COMPARE_MODE;

	TransTexWrapMode[TEXTURE_WRAP_REPEAT] = GL_REPEAT;
	TransTexWrapMode[TEXTURE_WRAP_CLAMP_TO_EDGE] = GL_CLAMP_TO_EDGE;
	TransTexWrapMode[TEXTURE_WRAP_CLAMP_TO_BORDER] = GL_CLAMP_TO_BORDER;
	TransTexWrapMode[TEXTURE_WRAP_MIRRORED_REPEAT] = GL_MIRRORED_REPEAT;

	TransTexMagMode[TEXTURE_MAG_NEAREST] = GL_NEAREST;
	TransTexMagMode[TEXTURE_MAG_LINEAR] = GL_LINEAR;

	TransTexMinMode[TEXTURE_MIN_NEAREST] = GL_NEAREST;
	TransTexMinMode[TEXTURE_MIN_LINEAR] = GL_LINEAR;
	TransTexMinMode[TEXTURE_MIN_NEAREST_MIPMAP_NEAREST] = GL_NEAREST_MIPMAP_NEAREST;
	TransTexMinMode[TEXTURE_MIN_NEAREST_MIPMAP_LINEAR] = GL_NEAREST_MIPMAP_LINEAR;
	TransTexMinMode[TEXTURE_MIN_LINEAR_MIPMAP_NEAREST] = GL_LINEAR_MIPMAP_NEAREST;
	TransTexMinMode[TEXTURE_MIN_LINEAR_MIPMAP_LINEAR] = GL_LINEAR_MIPMAP_LINEAR;

	TransTexCompareMode[COMPARE_NONE] = GL_NONE;
	TransTexCompareMode[COMPARE_LEQUAL] = GL_LEQUAL;
	TransTexCompareMode[COMPARE_GEQUAL] = GL_GEQUAL;
	TransTexCompareMode[COMPARE_LESS] = GL_LESS;
	TransTexCompareMode[COMPARE_GREATER] = GL_GREATER;
	TransTexCompareMode[COMPARE_EQUAL] = GL_EQUAL;
	TransTexCompareMode[COMPARE_NOTEQUAL] = GL_NOTEQUAL;
	TransTexCompareMode[COMPARE_ALWAYS] = GL_ALWAYS;
	TransTexCompareMode[COMPARE_NEVER] = GL_NEVER;

	TransTexCompareFunc[COMPARE_NONE] = GL_NONE;
	TransTexCompareFunc[COMPARE_LEQUAL] = GL_LEQUAL;
	TransTexCompareFunc[COMPARE_GEQUAL] = GL_GEQUAL;
	TransTexCompareFunc[COMPARE_LESS] = GL_LESS;
	TransTexCompareFunc[COMPARE_GREATER] = GL_GREATER;
	TransTexCompareFunc[COMPARE_EQUAL] = GL_EQUAL;
	TransTexCompareFunc[COMPARE_NOTEQUAL] = GL_NOTEQUAL;
	TransTexCompareFunc[COMPARE_ALWAYS] = GL_ALWAYS;
	TransTexCompareFunc[COMPARE_NEVER] = GL_NEVER;

	return true;
}


GLTextureSampler::GLTextureSampler(Texture *t): TextureSampler() {

#if CURITIBA_OPENGL_VERSION > 320
	glGenSamplers(1, &m_SamplerID);
#endif
	if (t->getMipmap()) {
		m_TexEnumProps[TEXTURE_MIN_FILTER] = TEXTURE_MIN_LINEAR_MIPMAP_LINEAR;
	}
	else
		m_TexEnumProps[TEXTURE_MIN_FILTER] = TEXTURE_MIN_LINEAR;

	m_TexEnumProps[TEXTURE_MAG_FILTER] = TEXTURE_MAG_LINEAR;
	m_TexEnumProps[TEXTURE_WRAP_S] = TEXTURE_WRAP_REPEAT;
	m_TexEnumProps[TEXTURE_WRAP_T] = TEXTURE_WRAP_REPEAT;
	m_TexEnumProps[TEXTURE_WRAP_R] = TEXTURE_WRAP_REPEAT;

	m_TexEnumProps[TEXTURE_COMPARE_MODE] = TEXTURE_COMPARE_NONE;
	m_TexEnumProps[TEXTURE_COMPARE_FUNC] = COMPARE_LEQUAL;

	float bordercolor[] = {-1.0f, -1.0f, -1.0f, -1.0f};
	m_TexFloat4Props[TEXTURE_BORDER_COLOR].set(bordercolor);

	update();

//	setMipmap(t->getMipmap());
}


void 
GLTextureSampler::update() {

#if CURITIBA_OPENGL_VERSION > 320
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, 
					translateTexMinMode((TextureMinMode)m_TexEnumProps[TEXTURE_MIN_FILTER]));
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_MAG_FILTER, 
					translateTexMinMode((TextureMinMode)m_TexEnumProps[TEXTURE_MAG_FILTER]));
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_S, 
					translateTexWrapMode((TextureWrapMode)m_TexEnumProps[TEXTURE_WRAP_S]));
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_T, 
					translateTexWrapMode((TextureWrapMode)m_TexEnumProps[TEXTURE_WRAP_T]));
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_R, 
					translateTexWrapMode((TextureWrapMode)m_TexEnumProps[TEXTURE_WRAP_R]));
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_COMPARE_FUNC, 
					translateTexCompareFunc((TextureCompareFunc)m_TexEnumProps[TEXTURE_COMPARE_FUNC]));
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_COMPARE_MODE,
					translateTexCompareMode((TextureCompareMode)m_TexEnumProps[TEXTURE_COMPARE_MODE]));
#endif
}


void 
GLTextureSampler::prepare(int aUnit, int aDim) {

#if (CURITIBA_OPENGL_VERSION > 320)
	glBindSampler(aUnit, getSamplerID());
#else
	int value;
	value = m_TexEnumProps[TEXTURE_WRAP_S] ;
	glTexParameteri(aDim, GL_TEXTURE_WRAP_S ,TransTexWrapMode[(TextureWrapMode)value]);

	value = m_TexEnumProps[TEXTURE_WRAP_R] ;
	glTexParameteri(aDim, GL_TEXTURE_WRAP_R ,TransTexWrapMode[(TextureWrapMode)value]);

	value = m_TexEnumProps[TEXTURE_WRAP_T] ;
	glTexParameteri(aDim, GL_TEXTURE_WRAP_T ,TransTexWrapMode[(TextureWrapMode)value]);

	value = m_TexEnumProps[TEXTURE_MIN_FILTER] ;
	glTexParameteri(aDim, GL_TEXTURE_MIN_FILTER ,TransTexMinMode[(TextureMinMode)value]);

	value = m_TexEnumProps[TEXTURE_MAG_FILTER] ;
	glTexParameteri(aDim, GL_TEXTURE_MAG_FILTER ,TransTexMagMode[(TextureMagMode)value]);

	value = m_TexEnumProps[TEXTURE_COMPARE_FUNC] ;
	glTexParameteri(aDim, GL_TEXTURE_COMPARE_FUNC ,TransTexCompareFunc[(TextureCompareFunc)value]);

	value = m_TexEnumProps[TEXTURE_COMPARE_MODE] ;
	glTexParameteri(aDim, GL_TEXTURE_COMPARE_MODE ,TransTexCompareMode[(TextureCompareMode)value]);

	vec4 v = m_TexFloat4Props[TEXTURE_BORDER_COLOR];
	glTexParameterfv(aDim, GL_TEXTURE_BORDER_COLOR,&(v.x));
#endif

}



// ---------------------------------------------------------------------
//
//								TEXTURE STATES
//
// ---------------------------------------------------------------------


void 
GLTextureSampler::setTexProp(TextureEnumProp prop, int value) {

	int v2;

	if (prop == TEXTURE_MIN_FILTER && m_Mipmap == true) { 
		if (value == TEXTURE_MIN_NEAREST)
			v2 = TEXTURE_MIN_NEAREST_MIPMAP_NEAREST;
		else if (value == TEXTURE_MIN_LINEAR)
			v2 = TEXTURE_MIN_LINEAR_MIPMAP_LINEAR;
	}
	else if (prop == TEXTURE_MIN_FILTER && m_Mipmap == false) {
		if (value == TEXTURE_MIN_NEAREST_MIPMAP_NEAREST)
			v2 = TEXTURE_MIN_NEAREST;
		else if (value == TEXTURE_MIN_NEAREST_MIPMAP_LINEAR || value == TEXTURE_MIN_LINEAR_MIPMAP_NEAREST || value == TEXTURE_MIN_LINEAR_MIPMAP_LINEAR)
			v2 = TEXTURE_MIN_LINEAR;

	}
	else 
		v2 = value;

	m_TexEnumProps[prop] = v2;

#if CURITIBA_OPENGL_VERSION > 320
	switch(prop) {
		case TEXTURE_WRAP_S:
		case TEXTURE_WRAP_T:
		case TEXTURE_WRAP_R:
			glSamplerParameteri(m_SamplerID, TransTexEnum[prop], translateTexWrapMode((TextureWrapMode)v2));
			break;
		case TEXTURE_MIN_FILTER:
			glSamplerParameteri(m_SamplerID, TransTexEnum[prop], translateTexMinMode((TextureMinMode)v2));
			break;
		case TEXTURE_MAG_FILTER:
			glSamplerParameteri(m_SamplerID, TransTexEnum[prop], translateTexMagMode((TextureMagMode)v2));
			break;
		case TEXTURE_COMPARE_MODE:
			glSamplerParameteri(m_SamplerID, TransTexEnum[prop], translateTexCompareMode((TextureCompareMode)v2));
			break;
		case TEXTURE_COMPARE_FUNC:
			glSamplerParameteri(m_SamplerID, TransTexEnum[prop], translateTexCompareFunc((TextureCompareFunc)v2));
			break;
	}
#endif
}


void 
GLTextureSampler::setTexProp(TextureFloat4Prop prop, float x, float y, float z, float w) {

	m_TexFloat4Props[prop].set(x,y,z,w);
}
			

void 
GLTextureSampler::setTexProp(TextureFloat4Prop prop, vec4& value) {

	m_TexFloat4Props[prop].set(value.x,value.y,value.z,value.w);
}
			

int 
GLTextureSampler::getTexProp(TextureEnumProp prop) {

	return(m_TexEnumProps[prop]);
}
			

vec4* 
GLTextureSampler::getTexProp(TextureFloat4Prop prop) {

	return(&m_TexFloat4Props[prop]);
}

// ---------------------------------------------------------------------
//
//								TRANSLATES
//
// ---------------------------------------------------------------------


//unsigned int 
//GLTextureSampler::TranslateTexEnum(GLTextureSampler::TextureEnumProp p) {
//
//	switch(p) {
//		case TEXTURE_WRAP_S:
//			return GL_TEXTURE_WRAP_S;
//		case TEXTURE_WRAP_T:
//			return GL_TEXTURE_WRAP_T;
//		case TEXTURE_WRAP_R:
//			return GL_TEXTURE_WRAP_R;
//		case TEXTURE_MIN_FILTER:
//			return GL_TEXTURE_MIN_FILTER;
//		case TEXTURE_MAG_FILTER:
//			return GL_TEXTURE_MAG_FILTER;
//		case TEXTURE_COMPARE_FUNC:
//			return GL_TEXTURE_COMPARE_FUNC;
//		case TEXTURE_COMPARE_MODE:
//			return GL_TEXTURE_COMPARE_MODE;
//		default:
//			return GL_INVALID_ENUM;
//	}
//}
	

//unsigned int  
//GLTextureSampler::TranslateTexWrapMode (TextureSampler::TextureWrapMode aMode)
//{
//	switch (aMode) {
//		case TEXTURE_WRAP_REPEAT:
//			return GL_REPEAT;
//		case TEXTURE_WRAP_CLAMP_TO_EDGE:
//			return GL_CLAMP_TO_EDGE;
//		case TEXTURE_WRAP_CLAMP_TO_BORDER:
//			return GL_CLAMP_TO_BORDER;
//		case TEXTURE_WRAP_MIRRORED_REPEAT:
//			return GL_MIRRORED_REPEAT;
//	        default:
//		  return GL_INVALID_ENUM;
//	}
//}


//unsigned int  
//GLTextureSampler::TranslateTexCompareMode (TextureSampler::TextureCompareMode aMode)
//{
//	switch (aMode) {
//		case TEXTURE_COMPARE_NONE:
//			return GL_NONE;
//		case TEXTURE_COMPARE_REF_TO_TEXTURE:
//			return GL_COMPARE_REF_TO_TEXTURE;
//	        default:
//		  return GL_INVALID_ENUM;
//	}
//}


//unsigned int  
//GLTextureSampler::TranslateTexMagMode (TextureSampler::TextureMagMode aMode)
//{
//	switch (aMode) {
//		case TEXTURE_MAG_NEAREST:
//			return GL_NEAREST;
//		case TEXTURE_MAG_LINEAR:
//			return GL_LINEAR;
//	        default:
//		  return GL_INVALID_ENUM;
//	}
//}


//unsigned int  
//GLTextureSampler::TranslateTexMinMode (TextureSampler::TextureMinMode aMode)
//{
//	switch (aMode) {
//		case TEXTURE_MIN_NEAREST:
//			return GL_NEAREST;
//		case TEXTURE_MIN_LINEAR:
//			return GL_LINEAR;
//		case TEXTURE_MIN_NEAREST_MIPMAP_NEAREST:
//			return GL_NEAREST_MIPMAP_NEAREST;
//		case TEXTURE_MIN_NEAREST_MIPMAP_LINEAR:
//			return GL_NEAREST_MIPMAP_LINEAR;
//		case TEXTURE_MIN_LINEAR_MIPMAP_NEAREST:
//			return GL_LINEAR_MIPMAP_NEAREST;
//		case TEXTURE_MIN_LINEAR_MIPMAP_LINEAR:
//			return GL_LINEAR_MIPMAP_LINEAR;
//	    default:
//		  return GL_INVALID_ENUM;
//	}
//}
//
//
//unsigned int  
//GLTextureSampler::TranslateTexCompareFunc (TextureSampler::TextureCompareFunc aFunc)
//{
//	switch (aFunc) {
//		case COMPARE_NONE:
//			return GL_NONE;
//		case COMPARE_LEQUAL:
//			return GL_LEQUAL;
//		case COMPARE_GEQUAL:
//			return GL_GEQUAL;
//		case COMPARE_LESS:
//			return GL_LESS;
//		case COMPARE_GREATER:
//			return GL_GREATER;
//		case COMPARE_EQUAL:
//			return GL_EQUAL;
//		case COMPARE_NOTEQUAL:
//			return GL_NOTEQUAL;
//		case COMPARE_ALWAYS:
//			return GL_ALWAYS;
//		case COMPARE_NEVER:
//			return GL_NEVER;
//	    default:
//		  return GL_INVALID_ENUM;
//	}
//}


//void 
//GLTextureSampler::setMipmap(bool m) {
//
//	m_Mipmap = m;
//
//	if (m_Mipmap && (m_TexEnumProps[TEXTURE_MIN_FILTER] == TEXTURE_MIN_NEAREST  || 
//						m_TexEnumProps[TEXTURE_MIN_FILTER] == TEXTURE_MIN_LINEAR)) {
//			m_TexEnumProps[TEXTURE_MIN_FILTER] = TEXTURE_MIN_LINEAR_MIPMAP_LINEAR;
//#if CURITIBA_OPENGL_VERSION > 320
//			glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, 
//					translateTexMinMode((TextureMinMode)m_TexEnumProps[TEXTURE_MIN_FILTER]));
//#endif
//	}
//
//	else if (!m_Mipmap && (m_TexEnumProps[TEXTURE_MIN_FILTER] != TEXTURE_MIN_NEAREST  && 
//						m_TexEnumProps[TEXTURE_MIN_FILTER] != TEXTURE_MIN_LINEAR)) {
//			m_TexEnumProps[TEXTURE_MIN_FILTER] = TEXTURE_MIN_LINEAR;
//#if CURITIBA_OPENGL_VERSION > 320
//			glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, 
//					translateTexMinMode((TextureMinMode)m_TexEnumProps[TEXTURE_MIN_FILTER]));
//#endif
//	}
//
//
//}