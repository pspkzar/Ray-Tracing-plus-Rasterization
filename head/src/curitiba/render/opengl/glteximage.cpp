#include <curitiba/render/opengl/glteximage.h>

using namespace curitiba::render;



GLTexImage::GLTexImage (Texture *t) :
	TexImage (t)
{
	int type = t->getType();
	int len = m_Width * m_Height * m_NumComponents; 

	switch (type) {
		case GL_FLOAT: 
			m_Data = (float *)malloc(sizeof(float) * len);
			update();
			break;
		case GL_UNSIGNED_INT:
			m_Data = (unsigned int *)malloc(sizeof(unsigned int) * len);
			update();
			break;
		case GL_UNSIGNED_SHORT:
			m_Data = (unsigned short *)malloc(sizeof(unsigned short) * len);
			update();
			break;
		case GL_UNSIGNED_BYTE:
			m_Data = ( unsigned char *)malloc(sizeof( unsigned char) * len);
			update();
			break;

		case GL_INT:
			m_Data = ( int *)malloc(sizeof( int) * len);
			update();
			break;
		case GL_BYTE:
			m_Data = (char *)malloc(sizeof(char) * len);
			update();
			break;
		case GL_SHORT:
			m_Data = ( short *)malloc(sizeof( short) * len);
			update();
			break;

		default:
			m_Data = NULL;
	}
}


GLTexImage::~GLTexImage(void)
{
	if (m_Data)
		free(m_Data);
}





void
GLTexImage::update(void) {
	
	glBindTexture(m_Texture->getDimension(),m_Texture->getId());
	glGetTexImage(m_Texture->getDimension(),0,m_Texture->getFormat(),m_Texture->getType(),m_Data);
}

void *
GLTexImage::getData() 
{
	return m_Data;
}

