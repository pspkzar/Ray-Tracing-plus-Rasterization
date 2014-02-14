#include <curitiba/render/rendertarget.h>

#include <curitiba/config.h>

#include <curitiba/render/opengl/glrendertarget.h>

using namespace curitiba::render;

RenderTarget* 
RenderTarget::create (std::string name, unsigned int width, unsigned int height)
{
#ifdef CURITIBA_OPENGL
	return new GLRenderTarget (name, width, height);
#elif CURITIBA_DIRECTX
	return new DXRenderTarget (width, height);
#endif
}

RenderTarget* 
RenderTarget::create (std::string name)
{
#ifdef CURITIBA_OPENGL
	return new GLRenderTarget (name);
#elif CURITIBA_DIRECTX
	return new DXRenderTarget ();
#endif
}

RenderTarget::RenderTarget (std::string name, unsigned int width, unsigned int height) :
	m_Id (0),
	m_Color(0),
	m_Depth(0),
	m_Stencil(0),
	m_Name (name),
	m_Width (width),
	m_Height (height),
	m_Samples(0)

{
	for (int i = 0; i < MAXFBOs+1; i++)
		m_TexId[i] = 0;
}


void
RenderTarget::setClearValues(float r, float g, float b, float a) 
{
	m_ClearValues.x = r;
	m_ClearValues.y = g;
	m_ClearValues.z = b;
	m_ClearValues.w = 0;
}


void 
RenderTarget::setSampleCount(int samples) 
{
	m_Samples = samples;
}


const curitiba::math::vec4 &
RenderTarget::getClearValues() 
{
	return m_ClearValues;
}


int
RenderTarget::getId (void)
{
	return m_Id;
}


std::string &
RenderTarget::getName (void)
{
	return m_Name;
}


unsigned int 
RenderTarget::getWidth (void)
{
	return m_Width;
}


unsigned int 
RenderTarget::getHeight (void) 
{
	return m_Height;
}


unsigned int 
RenderTarget::getNumberOfColorTargets() 
{
	return m_Color;
}


curitiba::render::Texture *
RenderTarget::getTexture(unsigned int i) 
{
	if (i <= MAXFBOs) {
		return m_TexId[i];
	} 
	else return 0;
}