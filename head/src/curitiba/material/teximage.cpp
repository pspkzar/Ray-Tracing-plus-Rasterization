#include <curitiba/material/teximage.h>

#ifdef CURITIBA_OPENGL
#include <curitiba/render/opengl/glteximage.h>
#endif

using namespace curitiba::material;


TexImage *
TexImage::create(Texture *t) 
{
	#ifdef CURITIBA_OPENGL
		return new GLTexImage(t);
	#elif CURITIBA_DIRECTX
		return new DXTexImage(t);
	#endif

}


TexImage::TexImage(Texture *t) 
{
	m_Texture = t;
	m_NumComponents = t->getNumberOfComponents();
	m_Width = t->getWidth();
	m_Height = t->getHeight();
	m_Depth = t->getDepth();
}


TexImage::~TexImage() 
{
	if (m_Data)
		delete(m_Data);
}


const std::string &
TexImage::getTextureName() 
{
	return m_Texture->getLabel();
}


std::string 
TexImage::getType() 
{
	return m_Texture->getStringType();
}


int 
TexImage::getWidth()
{
	return m_Texture->getWidth();
}


int 
TexImage::getHeight()
{
	return m_Texture->getHeight();
}


int 
TexImage::getDepth()
{
	return m_Texture->getDepth();
}


int 
TexImage::getNumComponents() 
{
	return m_NumComponents;
}
