#include <curitiba/render/opengl/gltexture.h>
#include <curitiba.h>
#include <curitiba/render/irenderer.h>

using namespace curitiba::render;

std::string GLTexture::StringType[] = {
								"UNSIGNED_BYTE", 
								"BYTE", 
								"UNSIGNED_SHORT", 
								"SHORT", 
								"UNSIGNED_INT",
								"INT",
								"FLOAT",
								"HALF_FLOAT",
								"UNSIGNED_INT_24_8"
								};

int GLTexture::Type[] = {
						GL_UNSIGNED_BYTE, 
						GL_BYTE, 
						GL_UNSIGNED_SHORT, 
						GL_SHORT,
						GL_UNSIGNED_INT,
						GL_INT,
						GL_FLOAT,
						GL_HALF_FLOAT,
						GL_UNSIGNED_INT_24_8,
						GL_INVALID_ENUM};
	
std::string GLTexture::StringFormat[] = {
								"LUMINANCE",
								"RED",
								"RG",
								"RGB",
								"RGBA",
								"DEPTH_COMPONENT",
								"DEPTH_STENCIL"
								};

int GLTexture::Format[] = {
						GL_LUMINANCE,
						GL_RED,
						GL_RG,
						GL_RGB,
						GL_RGBA,
						GL_DEPTH_COMPONENT,
						GL_DEPTH_STENCIL,
						GL_INVALID_ENUM};

std::string GLTexture::StringInternalFormat[] = {
								"LUMINANCE8",
								"LUMINANCE16",
								"R8",
								"R16",
								"RG8",
								"RG16",
								"RGB8",
								"RGB16",
								"RGBA",
								"RGBA8",
								"RGBA16", // 10
								"R32F",
								"RG16F",
								"RG32F",
								"RGBA16F",
								"RGBA32F",
								"R32I",
								"R32UI",
								"RG16I",
								"RG16UI",
								"RG32I", // 20
								"RG32UI",
								"RGBA8I",
								"RGBA8UI",
								"RGBA16I",
								"RGBA16UI",
								"RGBA32I",
								"RGBA32UI",
								"DEPTH_COMPONENT16",
								"DEPTH_COMPONENT24",
								"DEPTH_COMPONENT32", // 30
								"DEPTH_STENCIL",
								"DEPTH24_STENCIL8",
								};


int GLTexture::InternalFormat[] = {
								GL_LUMINANCE8,
								GL_LUMINANCE16,
								GL_R8,
								GL_R16,
								GL_RG8,
								GL_RG16,
								GL_RGB8,
								GL_RGB16,
								GL_RGBA8, //OPTIX STUFF
								GL_RGBA8,
								GL_RGBA16, // 10
								GL_R32F,
								GL_RG16F,
								GL_RG32F,
								GL_RGBA16F,
								GL_RGBA32F,
								GL_R32I,
								GL_R32UI,
								GL_RG16I,
								GL_RG16UI,
								GL_RG32I,  // 20
								GL_RG32UI,
								GL_RGBA8I,
								GL_RGBA8UI,
								GL_RGBA16I,
								GL_RGBA16UI,
								GL_RGBA32I,
								GL_RGBA32UI,
								GL_DEPTH_COMPONENT16,
								GL_DEPTH_COMPONENT24,
								GL_DEPTH_COMPONENT32, //30
								GL_DEPTH_STENCIL,
								GL_DEPTH24_STENCIL8,
								GL_INVALID_ENUM
								};

std::string GLTexture::StringDimension[] = {
								"TEXTURE_1D",
								"TEXTURE_2D",
								"TEXTURE_3D",
								"TEXTURE_CUBE_MAP",
								"TEXTURE_2D_MULTISAMPLE"
								};


int GLTexture::Dimension[] = {
								GL_TEXTURE_1D,
								GL_TEXTURE_2D,
								GL_TEXTURE_3D,
								GL_TEXTURE_CUBE_MAP,
								GL_TEXTURE_2D_MULTISAMPLE,
								GL_INVALID_ENUM
							};



bool
GLTexture::isValidFormat(std::string aFormat) 
{
	int res = getIndex(StringFormat, Format, aFormat);

	return (res != GL_INVALID_ENUM);
}

bool
GLTexture::isValidInternalFormat(std::string anInternalFormat) 
{
	int res = getIndex(StringInternalFormat, InternalFormat, anInternalFormat);

	return (res != GL_INVALID_ENUM);
}


//GLTexture::GLTexture (std::string label, std::string internalFormat, 
//					  std::string aFormat, std::string aType, int width, int height) :
//	Texture (label, "TEXTURE_2D", internalFormat, aFormat, aType, width, height)
//{
//	m_Dimension = GL_TEXTURE_2D;
//	m_Format = getIndex(StringFormat, Format, aFormat);
//	m_InternalFormat = getIndex(StringInternalFormat, InternalFormat, internalFormat);
//	m_Type = getIndex(StringType, Type, aType);
//
//	glGenTextures (1, &m_Id);
//	glBindTexture (m_Dimension, m_Id);
//	glTexImage2D (m_Dimension, 0, m_InternalFormat, 
//						m_IntProps[WIDTH], m_IntProps[HEIGHT], 0, m_Format, m_Type, 0);
//
//
//	//glTexParameteri(m_Dimension, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	//glTexParameteri(m_Dimension, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	//glTexParameterf(m_Dimension, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER/* _TO_EDGE*/);
//	//glTexParameterf(m_Dimension, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER/* _TO_EDGE*/);
//
//	//float bordercolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
//	//glTexParameterfv (m_Dimension, GL_TEXTURE_BORDER_COLOR, bordercolor/* _TO_EDGE*/);
//
//	glBindTexture (m_Dimension, 0);
//
//	m_Mipmap = false;
//}

	
	
GLTexture::GLTexture (std::string label, std::string internalFormat, std::string aFormat, 
		std::string aType, int width, int height, void* data, bool mipmap) :
	Texture (label, "TEXTURE_2D", internalFormat, aFormat, aType, width, height)
{
	m_Dimension = GL_TEXTURE_2D;
	m_Format = getIndex(StringFormat, Format, aFormat);
	m_InternalFormat = getIndex(StringInternalFormat, InternalFormat, internalFormat);
	m_Type = getIndex(StringType, Type, aType);

	glGenTextures (1, &m_Id);
	glBindTexture (m_Dimension, m_Id);

//#ifndef CURITIBA_OPTIX
//	glTexParameteri(m_Dimension, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//#else
//	glTexParameteri(m_Dimension, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//#endif
//	glTexParameteri(m_Dimension, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameterf(m_Dimension, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameterf(m_Dimension, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(m_Dimension, 0, m_InternalFormat , m_IntProps[WIDTH], m_IntProps[HEIGHT], 0,
 		m_Format, m_Type, data);


	if (data != NULL) {
		m_Mipmap = mipmap;
//#ifndef CURITIBA_OPTIX
		if (m_Mipmap)
			glGenerateMipmap(GL_TEXTURE_2D);
//#else
		//m_Mipmap = false;
//#endif
	}
	else
		m_Mipmap = false;
	glBindTexture (m_Dimension, 0);
}


GLTexture::GLTexture(std::string label) : Texture (label)
{

	m_Mipmap = false;
}


GLTexture::~GLTexture(void)
{
	glDeleteTextures (1, &m_Id);
}


bool
GLTexture::getMipmap() {

	return m_Mipmap;
}


int 
GLTexture::getElementSize() {

	int nComp = getNumberOfComponents();
	switch (m_Type) {
		case GL_FLOAT: 
			return nComp * sizeof(float);
			break;
		case GL_UNSIGNED_INT:
			return nComp * sizeof(unsigned int);
			break;
		case GL_UNSIGNED_SHORT:
			return nComp * sizeof(unsigned short);
			break;
		case GL_UNSIGNED_BYTE:
			return nComp * sizeof(unsigned char);
			break;
		case GL_INT:
			return nComp * sizeof(int);
			break;
		case GL_BYTE:
			return nComp * sizeof(char);
			break;
		case GL_SHORT:
			return nComp * sizeof(short);
			break;

	}
	return 0;
}



//void 
//GLTexture::setData(std::string internalFormat, std::string aFormat, std::string aType, 
//				   int width, int height) 
//{
//	m_StringDimension = "TEXTURE_2D";
//	m_StringInternalFormat = internalFormat;
//	m_StringFormat = aFormat;
//	m_StringType = aType;
//
//	m_Dimension = GL_TEXTURE_2D;
//	m_Format = getIndex(GLTexture::StringFormat, GLTexture::Format, aFormat);
//	m_InternalFormat = getIndex(GLTexture::StringInternalFormat, GLTexture::InternalFormat, internalFormat);
//	m_Type = getIndex(GLTexture::StringType, GLTexture::Type, aType);
//
//	//m_Width = width;
//	//m_Height = height;
//	//m_Depth = 1;
//	m_IntProps[WIDTH] = width;
//	m_IntProps[HEIGHT] = height;
//	m_IntProps[DEPTH] = 1;
//
//	glGenTextures (1, &m_Id);
//	glBindTexture (m_Dimension, m_Id);
//	glTexImage2D (m_Dimension, 0, m_InternalFormat, 
//						width, height, 0, m_Format, m_Type, 0);
//
////	glTexParameteri(m_Dimension, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////	glTexParameteri(m_Dimension, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//////	glTexParameterf(m_Dimension, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER/* _TO_EDGE*/);
//////	glTexParameterf(m_Dimension, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER/* _TO_EDGE*/);
////	glTexParameterf(m_Dimension, GL_TEXTURE_WRAP_S, GL_REPEAT/* _TO_EDGE*/);
////	glTexParameterf(m_Dimension, GL_TEXTURE_WRAP_T, GL_REPEAT/* _TO_EDGE*/);
////
////	float bordercolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
////	glTexParameterfv (m_Dimension, GL_TEXTURE_BORDER_COLOR, bordercolor/* _TO_EDGE*/);
//
//	glBindTexture (m_Dimension, 0);
//
//	m_Mipmap = false;
//}



// DAVE
void 
GLTexture::setData(std::string internalFormat, std::string aFormat, std::string aType, 
				   int width, int height, unsigned char * data) 
{
	m_StringDimension = "TEXTURE_2D";
	m_StringInternalFormat = internalFormat;
	m_StringFormat = aFormat;
	m_StringType = aType;

	m_Dimension = GL_TEXTURE_2D;
	m_Format = getIndex(GLTexture::StringFormat, GLTexture::Format, aFormat);
	m_InternalFormat = getIndex(GLTexture::StringInternalFormat, GLTexture::InternalFormat, internalFormat);
	m_Type = getIndex(GLTexture::StringType, GLTexture::Type, aType);

	//m_Width = width;
	//m_Height = height;
	//m_Depth = 1;
	m_IntProps[WIDTH] = width;
	m_IntProps[HEIGHT] = height;
	m_IntProps[DEPTH] = 1;

	glGenTextures (1, &m_Id);
	glBindTexture (m_Dimension, m_Id);
	glTexImage2D (m_Dimension, 0, m_InternalFormat, 
						width, height, 0, m_Format, m_Type, data);

	//glTexParameteri(m_Dimension, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//
	//glTexParameteri(m_Dimension, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//
	//glTexParameterf(m_Dimension, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER/* _TO_EDGE*/);
	//glTexParameterf(m_Dimension, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER/* _TO_EDGE*/);

	//float bordercolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	//glTexParameterfv (m_Dimension, GL_TEXTURE_BORDER_COLOR, bordercolor/* _TO_EDGE*/);

	glBindTexture (m_Dimension, 0);

	m_Mipmap = false;
}
// END DAVE


void 
GLTexture::prepare(int aUnit, TextureSampler *ts) {

	RENDERER->addTexture((IRenderer::TextureUnit)aUnit, this);
	Texture::TextureUnit tu = (Texture::TextureUnit)(Texture::TEXTURE0 + aUnit);
	m_SamplerProps[UNIT] = aUnit;
	glActiveTexture (GL_TEXTURE0+aUnit);
	glBindTexture(m_Dimension,m_Id);

	ts->prepare(aUnit, m_Dimension);

//#if (CURITIBA_OPENGL_VERSION > 320)
//	glBindSampler(aUnit, ts->getSamplerID());
//#else
//	int value;
//	value = ts->getTexProp(TextureSampler::TEXTURE_WRAP_S) ;
//	glTexParameteri(m_Dimension, GL_TEXTURE_WRAP_S ,GLTextureSampler::TransTexWrapMode[(TextureSampler::TextureWrapMode)value]);
//
//	value = ts->getTexProp(TextureSampler::TEXTURE_WRAP_R) ;
//	glTexParameteri(m_Dimension, GL_TEXTURE_WRAP_R ,GLTextureSampler::TransTexWrapMode[(TextureSampler::TextureWrapMode)value]);
//
//	value = ts->getTexProp(TextureSampler::TEXTURE_WRAP_T) ;
//	glTexParameteri(m_Dimension, GL_TEXTURE_WRAP_T ,GLTextureSampler::TransTexWrapMode[(TextureSampler::TextureWrapMode)value]);
//
//	value = ts->getTexProp(TextureSampler::TEXTURE_MIN_FILTER) ;
//	glTexParameteri(m_Dimension, GL_TEXTURE_MIN_FILTER ,GLTextureSampler::TransTexMinMode[(TextureSampler::TextureMinMode)value]);
//
//	value = ts->getTexProp(TextureSampler::TEXTURE_MAG_FILTER) ;
//	glTexParameteri(m_Dimension, GL_TEXTURE_MAG_FILTER ,GLTextureSampler::TransTexMagMode[(TextureSampler::TextureMagMode)value]);
//
//	value = ts->getTexProp(TextureSampler::TEXTURE_COMPARE_FUNC) ;
//	glTexParameteri(m_Dimension, GL_TEXTURE_COMPARE_FUNC ,GLTextureSampler::TransTexCompareFunc[(TextureSampler::TextureCompareFunc)value]);
//
//	value = ts->getTexProp(TextureSampler::TEXTURE_COMPARE_MODE) ;
//	glTexParameteri(m_Dimension, GL_TEXTURE_COMPARE_MODE ,GLTextureSampler::TransTexCompareMode[(TextureSampler::TextureCompareMode)value]);
//
//	vec4 *v = ts->getTexProp(TextureSampler::TEXTURE_BORDER_COLOR);
//	glTexParameterfv(m_Dimension, GL_TEXTURE_BORDER_COLOR,&v->x);
//#endif

}


void 
GLTexture::restore(int aUnit) {

	Texture::TextureUnit tu = (Texture::TextureUnit)(Texture::TEXTURE0 + aUnit);

	RENDERER->removeTexture((IRenderer::TextureUnit)aUnit);

	glActiveTexture (GL_TEXTURE0+aUnit);
	glBindTexture(m_Dimension,0);

#if (CURITIBA_OPENGL_VERSION > 320)

	glBindSampler(aUnit, 0);

#else
	glTexParameteri(m_Dimension, GL_TEXTURE_WRAP_S ,GL_REPEAT);
	glTexParameteri(m_Dimension, GL_TEXTURE_WRAP_R ,GL_REPEAT);
	glTexParameteri(m_Dimension, GL_TEXTURE_WRAP_T ,GL_REPEAT);

	glTexParameteri(m_Dimension, GL_TEXTURE_MIN_FILTER ,GL_LINEAR);
	glTexParameteri(m_Dimension, GL_TEXTURE_MAG_FILTER , GL_LINEAR);

	glTexParameteri(m_Dimension, GL_TEXTURE_COMPARE_FUNC ,GL_LEQUAL);
	glTexParameteri(m_Dimension, GL_TEXTURE_COMPARE_MODE ,GL_NONE);

	vec4 v(0.0, 0.0, 0.0, 0.0);
	glTexParameterfv(m_Dimension, GL_TEXTURE_BORDER_COLOR,&(v.x));
#endif
}


int
GLTexture::getNumberOfComponents(void) {

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
GLTexture::getDimension(void) 
{
	return m_Dimension;
}


int
GLTexture::getInternalFormat(void) 
{
	return m_InternalFormat;
}


int
GLTexture::getType(void) 
{
	return m_Type;
}


int 
GLTexture::getFormat(void)
{
	return m_Format;
}


int 
GLTexture::getIndex(std::string StringArray[], int IntArray[], std::string aString)
{
	int i;
	for (i = 0; (IntArray[i] != GL_INVALID_ENUM) && (StringArray[i].compare(aString)); i++) ;
//		i++;
	return IntArray[i];
}


// ---------------------------------------------------------------------
//
//								TRANSLATE ENUMS
//
// ---------------------------------------------------------------------


GLenum
GLTexture::translateCoord (TextureCoord aCoord)
{
	switch (aCoord) {
		case S_COORD:
			return GL_S;
		case T_COORD:
			return GL_T;
		case R_COORD:
			return GL_R;
		case Q_COORD:
			return GL_Q;
	    default:
		  return GL_INVALID_ENUM;
	}
}




//GLenum 
//GLTexture::translateTexWrapMode (TextureSampler::TextureWrapMode aMode)
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



//GLenum 
//GLTexture::translateTexCompareMode (TextureSampler::TextureCompareMode aMode)
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
//
//GLenum 
//GLTexture::translateTexMagMode (TextureSampler::TextureMagMode aMode)
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
//GLTexture::translateTexMinMode (TextureSampler::TextureMinMode aMode)
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
//GLTexture::translateTexCompareFunc (TextureSampler::TextureCompareFunc aFunc)
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



//GLenum 
//GLTexture::translateTexEnvMode (TextureSampler::TextureEnvMode aMode)
//{
//	switch (aMode) {
//		case IState::TEXTURE_ENV_MODULATE:
//			return GL_MODULATE;
//		case IState::TEXTURE_ENV_DECAL:
//			return GL_DECAL;
//		case IState::TEXTURE_ENV_BLEND:
//			return GL_BLEND;
//		case IState::TEXTURE_ENV_REPLACE:
//			return GL_REPLACE;
//	        default:
//		  return GL_INVALID_ENUM;
//	}
//}

//GLenum 
//GLTexture::translateDimension (TextureDimension aDimension)
//{
//
///*	if (aDimension == "TEXTURE_1D) 
//			return GL_TEXTURE_1D;
//	if (aDimension == "TEXTURE_1D) 
//			return GL_TEXTURE_2D;
//	if (aDimension == "TEXTURE_1D) 
//			return GL_TEXTURE_3D;
//	        default:
//		  return GL_INVALID_ENUM;
//	}
//	*/ return GL_INVALID_ENUM;
//}
//
//GLenum 
//GLTexture::translateFormat (TextureFormat aFormat)
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
//	    default:
//	        return GL_INVALID_ENUM;
//	}
//}
//
//
//GLenum 
//GLTexture::translateInternalFormat (TextureFormat aFormat)
//{
//	switch (aFormat) {
//		case DEPTH_COMPONENT:
//			return GL_DEPTH_COMPONENT;
//		case DEPTH_COMPONENT16:
//			return GL_DEPTH_COMPONENT16;
//		case DEPTH_COMPONENT24:
//			return GL_DEPTH_COMPONENT24;
//
//		case RGBA:
//			return GL_RGBA;
//		case RGBA8:
//			return GL_RGBA8;
//		case RGBA32F:
//			return GL_RGBA32F;//_ARB;
//
//		case RG16:
//			return GL_RG16;
//
//		case FLOAT32NV:
//			return GL_FLOAT_R32_NV;
//		case LUMINANCE:
//			return LUMINANCE;
//	    default:
//	        return GL_INVALID_ENUM;
//	}
//}
//
//GLenum 
//GLTexture::translateType (TextureType aType)
//{
//	switch (aType) {
//		case UNSIGNED_INT:
//			return GL_UNSIGNED_INT;
//		case UNSIGNED_BYTE:
//			return GL_UNSIGNED_BYTE;
//		case FLOAT:
//			return GL_FLOAT;
//	        default:
//	                return GL_INVALID_ENUM;
//	}
//}


//int 
//GLTexture::getIndex(std::string StringArray[], int IntArray[], std::string aString)
//{
//	int i;
//	for (i = 0; (IntArray[i] != GL_INVALID_ENUM); i++) {
//		std::string s =  StringArray[i];
//		if (!s.compare(aString))
//			break;
//	}
//	return IntArray[i];
//}


//void 
//GLTexture::bind (void)
//{
//	glBindTexture (translateDimension (m_Dimension), m_Id);
//}
//
//void 
//GLTexture::unbind (void)
//{
//	glBindTexture (translateDimension (m_Dimension), 0);
//}
/*
void 
GLTexture::enableCompareToTexture (void)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
}

void 
GLTexture::disableCompareToTexture (void)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_NONE);
}
*/





//void 
//GLTexture::enableObjectSpaceCoordGen (void)
//{
//	glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
//	glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
//}
//
//void 
//GLTexture::generateObjectSpaceCoords (TextureCoord aCoord, float *plane)
//{
//	glTexGenfv (translateCoord (aCoord), GL_OBJECT_PLANE, plane);
//}

//void 
//GLTexture::prepare(int aUnit, curitiba::render::IState *state) {
//
////	int value;
////
////	RENDERER->addTexture((IRenderer::TextureUnit)aUnit, this);
////	IState::TextureUnit tu = (IState::TextureUnit)(IState::TEXTURE0 + aUnit);
////	m_SamplerProps[UNIT] = aUnit;
////	glActiveTexture (GL_TEXTURE0+aUnit);
////	glBindTexture(m_Dimension,m_Id);
////
////	value = state->getTexProp(tu,IState::TEXTURE_WRAP_S) ;
////	if (value) 
////		glTexParameteri(m_Dimension, GL_TEXTURE_WRAP_S ,translateTexWrapMode((IState::TextureWrapMode)value));
////
////	value = state->getTexProp(tu,IState::TEXTURE_WRAP_R) ;
////	if (value) 
////		glTexParameteri(m_Dimension, GL_TEXTURE_WRAP_R ,translateTexWrapMode((IState::TextureWrapMode)value));
////
////	value = state->getTexProp(tu,IState::TEXTURE_WRAP_T) ;
////	if (value) 
////		glTexParameteri(m_Dimension, GL_TEXTURE_WRAP_T ,translateTexWrapMode((IState::TextureWrapMode)value));
////
////	value = state->getTexProp(tu,IState::TEXTURE_MIN_FILTER) ;
////	if (value) 
////		glTexParameteri(m_Dimension, GL_TEXTURE_MIN_FILTER ,translateTexMinMode((IState::TextureMinMode)value));
////
////	value = state->getTexProp(tu,IState::TEXTURE_MAG_FILTER) ;
////	if (value) 
////		glTexParameteri(m_Dimension, GL_TEXTURE_MAG_FILTER ,translateTexMagMode((IState::TextureMagMode)value));
////
////	value = state->getTexProp(tu,IState::TEXTURE_COMPARE_FUNC) ;
////	if (value) 
////		glTexParameteri(m_Dimension, GL_TEXTURE_COMPARE_FUNC ,translateTexCompareFunc((IState::TextureCompareFunc)value));
////
////	value = state->getTexProp(tu,IState::TEXTURE_COMPARE_MODE) ;
////	if (value) 
////		glTexParameteri(m_Dimension, GL_TEXTURE_COMPARE_MODE ,translateTexCompareMode((IState::TextureCompareMode)value));
////
////
////	//vec4 *v = state->getTexProp(tu,IState::TEXTURE_BORDER_COLOR);
////	//glTexParameterfv(m_Dimension, GL_TEXTURE_BORDER_COLOR,&v->x);
////
////#if CURITIBA_OPENGL_VERSIONOPENGL_VERSION < 400
////	value = state->getTexProp(tu,IState::TEXTURE_ENV_MODE) ;
////	if (value) 
////		glTexParameteri(m_Dimension, GL_TEXTURE_ENV_MODE ,translateTexEnvMode((IState::TextureEnvMode)value));
////#endif
//}

//void 
//GLTexture::restore(int aUnit, curitiba::render::IState *state) {
//
//	//int value;
//	//
//	//IState::TextureUnit tu = (IState::TextureUnit)(IState::TEXTURE0 + aUnit);
//
//	//RENDERER->removeTexture((IRenderer::TextureUnit)aUnit);
//
//	//glActiveTexture (GL_TEXTURE0+aUnit);
//	//glBindTexture(m_Dimension,m_Id);
//
//	//value = state->getTexProp(tu,IState::TEXTURE_WRAP_S) ;
//	//if (value) 
//	//	glTexParameteri(m_Dimension, GL_TEXTURE_WRAP_S ,translateTexWrapMode((IState::TextureWrapMode)0));
//
//	//value = state->getTexProp(tu,IState::TEXTURE_WRAP_R) ;
//	//if (value) 
//	//	glTexParameteri(m_Dimension, GL_TEXTURE_WRAP_R ,translateTexWrapMode((IState::TextureWrapMode)0));
//
//	//value = state->getTexProp(tu,IState::TEXTURE_WRAP_T) ;
//	//if (value) 
//	//	glTexParameteri(m_Dimension, GL_TEXTURE_WRAP_T ,translateTexWrapMode((IState::TextureWrapMode)0));
//
//	//value = state->getTexProp(tu,IState::TEXTURE_MIN_FILTER) ;
//	//if (value) 
//	//	glTexParameteri(m_Dimension, GL_TEXTURE_MIN_FILTER ,translateTexMinMode((IState::TextureMinMode)0));
//
//	//value = state->getTexProp(tu,IState::TEXTURE_MAG_FILTER) ;
//	//if (value) 
//	//	glTexParameteri(m_Dimension, GL_TEXTURE_MAG_FILTER ,translateTexMagMode((IState::TextureMagMode)0));
//
//	//value = state->getTexProp(tu,IState::TEXTURE_COMPARE_FUNC) ;
//	//if (value) 
//	//	glTexParameteri(m_Dimension, GL_TEXTURE_COMPARE_FUNC ,translateTexCompareFunc((IState::TextureCompareFunc)0));
//
//	//value = state->getTexProp(tu,IState::TEXTURE_COMPARE_MODE) ;
//	//if (value) 
//	//	glTexParameteri(m_Dimension, GL_TEXTURE_COMPARE_MODE ,translateTexCompareMode((IState::TextureCompareMode)0));
//
//	//value = state->getTexProp(tu,IState::TEXTURE_ENV_MODE) ;
//	//if (value) 
//	//	glTexParameteri(m_Dimension, GL_TEXTURE_ENV_MODE ,translateTexEnvMode((IState::TextureEnvMode)0));
//
//	//vec4 v;
//	//v.set(1.0f,1.0f,1.0f,1.0f);
//	//glTexParameterfv(m_Dimension, GL_TEXTURE_BORDER_COLOR,&v.x);
//	//glBindTexture(m_Dimension,0);
//}
