#include <curitiba/render/texture.h>
#include <curitiba/config.h>

#ifdef CURITIBA_OPENGL
#include <curitiba/render/opengl/gltexture.h>
#include <curitiba/render/opengl/gltextureMS.h>
#endif
#include <curitiba/loader/textureloader.h>


using namespace curitiba::render;
using namespace curitiba::loader;


const std::string Texture::IntString[] = {"WIDTH", "HEIGHT", "DEPTH", "SAMPLES"};
const std::string Texture::SamplerString[] = {"UNIT" };

void 
Texture::getPropTypeAndId(std::string &s, curitiba::Enums::DataType *dt , int *id) 
{
	// value returned in case of an invalid string
	*id = -1;

	for (int i = 0; i < COUNT_INTPROPERTY; i++) {

		if (s == IntString[i]) {
		
			*id = i;
			*dt = Enums::INT;
			return;
		}
	}
	for (int i = 0; i < COUNT_SAMPLERPROPERTY; i++) {

		if (s == SamplerString[i]) {
		
			*id = i;
			*dt = Enums::SAMPLER;
			return;
		}
	}

}


//Texture*
//Texture::create (std::string label,std::string internalFormat, std::string aFormat, 
//				 std::string aType, int width, int height)
//{
//
//#ifdef CURITIBA_OPENGL
//	return new GLTexture (label, internalFormat, aFormat, aType, width, height);
//#elif CURITIBA_DIRECTX
//	return new DXTexture (aDimension, aFormat, width, height);
//#endif
//}

Texture*
Texture::create (std::string label, std::string internalFormat,
				std::string aFormat, std::string aType, int width, int height, 
				unsigned char* data)
{
#ifdef CURITIBA_OPENGL
	return new GLTexture (label, internalFormat, aFormat, aType, width, height, data);
#elif CURITIBA_DIRECTX
	//Meter função para DirectX
#endif
}

Texture*
Texture::createMS (std::string label, std::string internalFormat,
				int width, int height, 
				int samples)
{
#ifdef CURITIBA_OPENGL
	return new GLTextureMS (label, internalFormat, width, height, samples);
#elif CURITIBA_DIRECTX
	//Meter função para DirectX
#endif
}



Texture*
Texture::create (std::string label)
{

#ifdef CURITIBA_OPENGL
	return new GLTexture (label);
#elif CURITIBA_DIRECTX
	return new DXTexture (aDimension, aFormat, width, height);
#endif
}


Texture*
Texture::create (std::string file, std::string label, bool mipmap)
{
	TextureLoader *loader = TextureLoader::create();

	int success = loader->loadImage (file);
	if (success) {
		std::string aFormat = loader->getFormat();
		curitiba::render::Texture *t;

	#ifdef CURITIBA_OPENGL
		t = new GLTexture (label, aFormat, aFormat, loader->getType(), 
				loader->getWidth(), loader->getHeight(), loader->getData(), mipmap);
	#elif CURITIBA_DIRECTX
		t = new DXTexture (aDimension, aFormat, width, height);
	#endif

	#ifdef __SLANGER__
		ilConvertImage(IL_RGB,IL_UNSIGNED_BYTE);
		iluScale(96,96,1);
	
		t->bitmap = new wxBitmap(wxImage(96,96,loader->getData(),true).Mirror(false));
	#endif

		loader->freeImage();
		delete loader;
		return t;
	}
	else {
		delete loader;
		return NULL;
	}

}

Texture::Texture(std::string label, std::string aDimension, std::string internalFormat, 
				 std::string aFormat, std::string aType, int width, int height) :
	m_Id (0),
	m_IntProps(COUNT_INTPROPERTY),
	m_SamplerProps(COUNT_SAMPLERPROPERTY),
	m_StringDimension (aDimension),
	m_StringInternalFormat (internalFormat),
	m_StringFormat (aFormat),
	m_StringType (aType),
	m_Label (label)
#ifdef __SLANGER__
	,bitmap(0)
#endif
{
	m_IntProps[WIDTH] = width;
	m_IntProps[HEIGHT] = height;
	m_IntProps[DEPTH] = 1;
	m_IntProps[SAMPLES] = 1;
	m_SamplerProps[UNIT] = 0;
}


Texture::Texture(std::string label): 
	m_Id (0),
	m_IntProps(COUNT_INTPROPERTY),
	m_SamplerProps(COUNT_SAMPLERPROPERTY),
	m_Label (label)
#ifdef __SLANGER__
	,bitmap(0)
#endif	
{
	m_IntProps[WIDTH] = 0;
	m_IntProps[HEIGHT] = 0;
	m_IntProps[DEPTH] = 0;
	m_IntProps[SAMPLES] = 1;
	m_SamplerProps[UNIT] = 0;

}


Texture::~Texture(){

#ifdef __SLANGER__

	if (bitmap)
		delete bitmap;	

#endif
}

bool
Texture::isValidFormat(std::string aFormat) 
{
	#ifdef CURITIBA_OPENGL
		return GLTexture::isValidFormat(aFormat);
	#elif CURITIBA_DIRECTX
		return DXTexture::isValidFormat(aFormat);
	#endif
}


bool
Texture::isValidInternalFormat(std::string aFormat) 
{
	#ifdef CURITIBA_OPENGL
		return GLTexture::isValidInternalFormat(aFormat);
	#elif CURITIBA_DIRECTX
		return DXTexture::isValidInternalFormat(aFormat);
	#endif
}

int 
Texture::getArraySize(int IntArray[]) 
{
	int i;
	// 1280 is GL_INVALID_ENUM!!! Must Remove this dependency
	for (i = 0; IntArray[i] != 1280; i++);
	return i;
}


int 
Texture::getPropi(IntProperty prop)
{
	return m_IntProps[prop];
}

int 
Texture::getProps(SamplerProperty prop)
{
	return m_SamplerProps[prop];
}


std::string*
Texture::getStringDimensions(void) 
{
	#ifdef CURITIBA_OPENGL
		return GLTexture::StringDimension;
	#elif CURITIBA_DIRECTX
		t = new DXTexture (aDimension, aFormat, width, height);
	#endif
}

int*
Texture::getDimensions(void) 
{
	#ifdef CURITIBA_OPENGL
		return GLTexture::Dimension;
	#elif CURITIBA_DIRECTX
		t = new DXTexture (aDimension, aFormat, width, height);
	#endif
}

std::string*
Texture::getStringTypes(void) 
{
	#ifdef CURITIBA_OPENGL
		return GLTexture::StringType;
	#elif CURITIBA_DIRECTX
		t = new DXTexture (aDimension, aFormat, width, height);
	#endif
}

int*
Texture::getTypes(void) 
{
	#ifdef CURITIBA_OPENGL
		return GLTexture::Type;
	#elif CURITIBA_DIRECTX
		t = new DXTexture (aDimension, aFormat, width, height);
	#endif
}

std::string*
Texture::getStringFormats(void) 
{
	#ifdef CURITIBA_OPENGL
		return GLTexture::StringFormat;
	#elif CURITIBA_DIRECTX
		t = new DXTexture (aDimension, aFormat, width, height);
	#endif
}

int*
Texture::getFormats(void) 
{
	#ifdef CURITIBA_OPENGL
		return GLTexture::Format;
	#elif CURITIBA_DIRECTX
		t = new DXTexture (aDimension, aFormat, width, height);
	#endif
}

std::string*
Texture::getStringInternalFormats(void) 
{
	#ifdef CURITIBA_OPENGL
		return GLTexture::StringInternalFormat;
	#elif CURITIBA_DIRECTX
		t = new DXTexture (aDimension, aFormat, width, height);
	#endif
}

int*
Texture::getInternalFormats(void) 
{
	#ifdef CURITIBA_OPENGL
		return GLTexture::InternalFormat;
	#elif CURITIBA_DIRECTX
		t = new DXTexture (aDimension, aFormat, width, height);
	#endif
}


#ifdef __SLANGER__

wxBitmap *
Texture::getBitmap(void) {
	return bitmap;	
}
#endif

std::string 
Texture::getStringDimension(void) {

	return(m_StringDimension);
}


void 
Texture::setId (int id)
{
	m_Id = id;
}

int 
Texture::getId (void)
{
	return m_Id;
}

std::string&
Texture::getLabel (void)
{
	return m_Label;
}

void
Texture::setLabel (std::string label)
{
	m_Label = label;
}

int 
Texture::getWidth (void)
{
	return m_IntProps[WIDTH];
}

int 
Texture::getHeight (void)
{
	return m_IntProps[HEIGHT];
}

int
Texture::getDepth(void) 
{
	return m_IntProps[DEPTH];
}

std::string 
Texture::getStringFormat (void)
{
	return m_StringFormat;
}

std::string 
Texture::getStringInternalFormat (void)
{
	return m_StringInternalFormat;
}

std::string 
Texture::getStringType (void)
{
	return m_StringType;
}

void 
Texture::setUnit (int unit)
{
	m_SamplerProps[UNIT] = unit;
}

int 
Texture::getUnit (void)
{
	return m_SamplerProps[UNIT];
}


