#include <curitiba/render/opengl/gltextureMS.h>



	
	
GLTextureMS::GLTextureMS (std::string label, std::string internalFormat, int width, int height, int samples) :
	GLTexture (label)
{
	m_IntProps[WIDTH] = width;
	m_IntProps[HEIGHT] = height;
	m_IntProps[DEPTH] = 1;
	m_IntProps[SAMPLES] = samples;

	m_Dimension = GL_TEXTURE_2D_MULTISAMPLE;
	m_Format = getIndex(StringFormat, Format, "RGBA");
	m_InternalFormat = getIndex(StringInternalFormat, InternalFormat, internalFormat);
	m_Type = getIndex(StringType, Type, "FLOAT");

	glGenTextures (1, &m_Id);
	glBindTexture (m_Dimension, m_Id);

	glTexImage2DMultisample(m_Dimension, samples, m_InternalFormat , m_IntProps[WIDTH], m_IntProps[HEIGHT], true);

	m_Mipmap = false;

	glBindTexture (m_Dimension, 0);
}



GLTextureMS::~GLTextureMS(void)
{
	glDeleteTextures (1, &m_Id);
}

