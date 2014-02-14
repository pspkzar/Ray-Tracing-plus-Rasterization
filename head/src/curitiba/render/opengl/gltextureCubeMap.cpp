#include <curitiba/render/opengl/gltextureCubeMap.h>
#include <curitiba/render/opengl/gltexture.h>
#include <curitiba/math/mat4.h>

using namespace curitiba::render;

int GLTextureCubeMap::faces[6] = {
			GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
			};


GLTextureCubeMap::GLTextureCubeMap (std::string label, std::vector<std::string> files, 
									std::string internalFormat, std::string aFormat, 
									std::string aType, int width, unsigned char** data, bool mipmap) :
	TextureCubeMap (label,files, internalFormat, aFormat, aType, width)
{

	m_Format = getIndex(GLTexture::StringFormat, GLTexture::Format, aFormat);
	m_InternalFormat = getIndex(GLTexture::StringInternalFormat, GLTexture::InternalFormat, internalFormat);
	m_Type = getIndex(GLTexture::StringType, GLTexture::Type, aType);


	glGenTextures (1, &m_Id);
	glBindTexture (GL_TEXTURE_CUBE_MAP, m_Id);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);

	for(int i = 0 ; i < 6 ; i++) {
		
		glTexImage2D (GLTextureCubeMap::faces[i], 0, m_InternalFormat, 
						m_IntProps[WIDTH], m_IntProps[HEIGHT], 0, m_Format, m_Type, data[i]);
	}
	m_Mipmap = mipmap;
	if (mipmap)
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

#if CURITIBA_CORE_OPENGL == 0
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
#endif
	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
}


GLTextureCubeMap::~GLTextureCubeMap(void)
{
	glDeleteTextures (1, &m_Id);
}

int 
GLTextureCubeMap::getIndex(std::string StringArray[], int IntArray[], std::string aString)
{
	int i;
	for (i = 0; IntArray[i] != GL_INVALID_ENUM && StringArray[i] != aString; i++) 
		i++;
	return IntArray[i];
}

int
GLTextureCubeMap::getNumberOfComponents(void) {

	switch(m_Format) {

		case GL_LUMINANCE:
		case GL_RED:
		case GL_DEPTH_COMPONENT:
			return 1;
		case GL_RG:
		case GL_DEPTH_STENCIL:
			return 2;
		case GL_RGB:
			return 3;
		case GL_RGBA:
			return 4;
		default:
			return 0;
	}
}


int
GLTextureCubeMap::getDimension(void) 
{
	return GL_TEXTURE_CUBE_MAP;
}

int
GLTextureCubeMap::getInternalFormat(void) 
{
	return m_InternalFormat;
}

int
GLTextureCubeMap::getType(void) 
{
	return m_Type;
}

int 
GLTextureCubeMap::getFormat(void)
{
	return m_Format;
}


void 
GLTextureCubeMap::prepare(int aUnit, curitiba::material::TextureSampler *ts) {

	RENDERER->addTexture((IRenderer::TextureUnit)aUnit, this);
	Texture::TextureUnit tu = (Texture::TextureUnit)(Texture::TEXTURE0 + aUnit);
	m_SamplerProps[UNIT] = aUnit;
	glActiveTexture (GL_TEXTURE0+aUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP,m_Id);
	glBindSampler(aUnit, ts->getSamplerID());

	ts->prepare(aUnit, GL_TEXTURE_CUBE_MAP);

//
//#if CURITIBA_OPENGL_VERSION > 320
//	glBindSampler(aUnit, ts->getSamplerID());
//#else
//	int value;
//	value = ts->getTexProp(TextureSampler::TEXTURE_WRAP_S) ;
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S ,GLTextureSampler::TransTexWrapMode[(TextureSampler::TextureWrapMode)value]);
//
//	value = ts->getTexProp(TextureSampler::TEXTURE_WRAP_R) ;
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R ,GLTextureSampler::TransTexWrapMode[(TextureSampler::TextureWrapMode)value]);
//
//	value = ts->getTexProp(TextureSampler::TEXTURE_WRAP_T) ;
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T ,GLTextureSampler::TransTexWrapMode[(TextureSampler::TextureWrapMode)value]);
//
//	value = ts->getTexProp(TextureSampler::TEXTURE_MIN_FILTER) ;
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER ,GLTextureSampler::TransTexMinMode[(TextureSampler::TextureMinMode)value]);
//
//	value = ts->getTexProp(TextureSampler::TEXTURE_MAG_FILTER) ;
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER ,GLTextureSampler::TransTexMagMode[(TextureSampler::TextureMagMode)value]);
//
//	value = ts->getTexProp(TextureSampler::TEXTURE_COMPARE_FUNC) ;
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC ,GLTextureSampler::TransTexCompareFunc[(TextureSampler::TextureCompareFunc)value]);
//
//	value = ts->getTexProp(TextureSampler::TEXTURE_COMPARE_MODE) ;
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE ,GLTextureSampler::TransTexCompareMode[(TextureSampler::TextureCompareMode)value]);
//
//	vec4 *v = ts->getTexProp(TextureSampler::TEXTURE_BORDER_COLOR);
//	glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR,&v->x);
//
//
//#endif
}


void 
GLTextureCubeMap::restore(int aUnit) 
{
	Texture::TextureUnit tu = (Texture::TextureUnit)(Texture::TEXTURE0 + aUnit);

	glActiveTexture (GL_TEXTURE0+aUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP,0);

#if CURITIBA_OPENGL_VERSION > 320
	glBindSampler(aUnit, 0);
#endif
#if CURITIBA_CORE_OPENGL == 0
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);

	glDisable(GL_TEXTURE_CUBE_MAP);
#endif
}



//GLenum 
//GLTextureCubeMap::translateTexWrapMode (TextureSampler::TextureWrapMode aMode)
//{
//	switch (aMode) {
//		case IState::TEXTURE_WRAP_REPEAT:
//			return GL_REPEAT;
//		case IState::TEXTURE_WRAP_CLAMP_TO_EDGE:
//			return GL_CLAMP_TO_EDGE;
//		case IState::TEXTURE_WRAP_CLAMP_TO_BORDER:
//			return GL_CLAMP_TO_BORDER;
//		case IState::TEXTURE_WRAP_MIRRORED_REPEAT:
//			return GL_MIRRORED_REPEAT;
//	        default:
//		  return GL_INVALID_ENUM;
//	}
//}
//
//
//
//GLenum 
//GLTextureCubeMap::translateTexMagMode (TextureSampler::TextureMagMode aMode)
//{
//	switch (aMode) {
//		case IState::TEXTURE_MAG_NEAREST:
//			return GL_NEAREST;
//		case IState::TEXTURE_MAG_LINEAR:
//			return GL_LINEAR;
//	        default:
//		  return GL_INVALID_ENUM;
//	}
//}
//
//GLenum 
//GLTextureCubeMap::translateTexMinMode (TextureSampler::TextureMinMode aMode)
//{
//	switch (aMode) {
//		case IState::TEXTURE_MIN_NEAREST:
//			return GL_NEAREST;
//		case IState::TEXTURE_MIN_LINEAR:
//			return GL_LINEAR;
//		case IState::TEXTURE_MIN_NEAREST_MIPMAP_NEAREST:
//			return GL_NEAREST_MIPMAP_NEAREST;
//		case IState::TEXTURE_MIN_NEAREST_MIPMAP_LINEAR:
//			return GL_NEAREST_MIPMAP_LINEAR;
//		case IState::TEXTURE_MIN_LINEAR_MIPMAP_NEAREST:
//			return GL_LINEAR_MIPMAP_NEAREST;
//		case IState::TEXTURE_MIN_LINEAR_MIPMAP_LINEAR:
//			return GL_LINEAR_MIPMAP_LINEAR;
//	    default:
//		  return GL_INVALID_ENUM;
//	}
//}
//
//GLenum 
//GLTextureCubeMap::translateTexCompareFunc (TextureSampler::TextureCompareFunc aFunc)
//{
//	switch (aFunc) {
//		case IState::COMPARE_NONE:
//			return GL_NONE;
//		case IState::COMPARE_LEQUAL:
//			return GL_LEQUAL;
//		case IState::COMPARE_GEQUAL:
//			return GL_GEQUAL;
//		case IState::COMPARE_LESS:
//			return GL_LESS;
//		case IState::COMPARE_GREATER:
//			return GL_GREATER;
//		case IState::COMPARE_EQUAL:
//			return GL_EQUAL;
//		case IState::COMPARE_NOTEQUAL:
//			return GL_NOTEQUAL;
//		case IState::COMPARE_ALWAYS:
//			return GL_ALWAYS;
//		case IState::COMPARE_NEVER:
//			return GL_NEVER;
//	    default:
//		  return GL_INVALID_ENUM;
//	}
//}
//
//GLenum 
//GLTextureCubeMap::translateTexCompareMode (TextureSampler::TextureCompareMode aMode)
//{
//	switch (aMode) {
//		case IState::TEXTURE_COMPARE_NONE:
//			return GL_NONE;
//		case IState::TEXTURE_COMPARE_REF_TO_TEXTURE:
//			return GL_COMPARE_REF_TO_TEXTURE;
//	        default:
//		  return GL_INVALID_ENUM;
//	}
//}

//GLenum 
//GLTextureCubeMap::translateType (TextureType aType)
//{
//	switch (aType) {
//		case UNSIGNED_INT:
//			return GL_UNSIGNED_INT;
//		case UNSIGNED_BYTE:
//			return GL_UNSIGNED_BYTE;
//		case FLOAT:
//			return GL_FLOAT;
//	    default:
//	        return GL_INVALID_ENUM;
//	}
//}
//
//GLenum 
//GLTextureCubeMap::translateFormat (TextureFormat aFormat)
//{
//	switch (aFormat) {
//		case DEPTH_COMPONENT:
//			return GL_DEPTH_COMPONENT;
//		case DEPTH_COMPONENT16:
//			return GL_DEPTH_COMPONENT16;
//		case DEPTH_COMPONENT24:
//			return GL_DEPTH_COMPONENT24;
//		case RGBA:
//			return GL_RGBA;
//		case RGBA8:
//			return GL_RGBA8;
//		case RGBA32F:
//			return GL_RGBA32F_ARB;
//		case FLOAT32NV:
//			return GL_FLOAT_R32_NV;
//		case LUMINANCE:
//			return LUMINANCE;
//	        default:
//	                return GL_INVALID_ENUM;
//	}
//}
//void 
//GLTextureCubeMap::prepare(int aUnit, curitiba::render::IState *state) 
//{
//	IState::TextureUnit tu = (IState::TextureUnit)(IState::TEXTURE0 + aUnit);
//
//	glActiveTexture (GL_TEXTURE0+aUnit);
//	glBindTexture(GL_TEXTURE_CUBE_MAP,m_Id);
//#if CURITIBA_OPENGL_VERSION < 400
//	glEnable(GL_TEXTURE_CUBE_MAP);
//	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
//	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
//	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
//
//
//	curitiba::scene::Camera *theCam = RENDERMANAGER->getCurrentCamera();
//	glMatrixMode(GL_TEXTURE);
//	mat4 m;
//	m.copy(theCam->getPropm4(Camera::VIEW_INVERSE_MATRIX).getMatrix());
//	m.set(3,0,0.0);
//	m.set(3,1,0.0);
//	m.set(3,2,0.0);
//	glLoadMatrixf(m.getMatrix());
//	glMatrixMode(GL_MODELVIEW);
//
//	glEnable(GL_TEXTURE_GEN_S);
//    glEnable(GL_TEXTURE_GEN_T);
//    glEnable(GL_TEXTURE_GEN_R);
//#endif
//}
//
//void 
//GLTextureCubeMap::restore(int aUnit, curitiba::render::IState *state) 
//{
//	IState::TextureUnit tu = (IState::TextureUnit)(IState::TEXTURE0 + aUnit);
//
//	glActiveTexture (GL_TEXTURE0+aUnit);
//	glBindTexture(GL_TEXTURE_CUBE_MAP,0);
//#if CURITIBA_OPENGL_VERSION < 400
//	glMatrixMode(GL_TEXTURE);
//	glLoadIdentity();
//	glMatrixMode(GL_MODELVIEW);
//
//	glDisable(GL_TEXTURE_GEN_S);
//    glDisable(GL_TEXTURE_GEN_T);
//    glDisable(GL_TEXTURE_GEN_R);
//
//	glDisable(GL_TEXTURE_CUBE_MAP);
//#endif
//}



//void 
//GLTextureCubeMap::bind (void)
//{
//	glBindTexture (GL_TEXTURE_CUBE_MAP, m_Id);
//}
//
//void 
//GLTextureCubeMap::unbind (void)
//{
//	glBindTexture (GL_TEXTURE_CUBE_MAP, m_Id);
//	glDisable(GL_TEXTURE_GEN_S);
//    glDisable(GL_TEXTURE_GEN_T);
//    glDisable(GL_TEXTURE_GEN_R);
//	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
//	glDisable(GL_TEXTURE_CUBE_MAP);
//
//
//}