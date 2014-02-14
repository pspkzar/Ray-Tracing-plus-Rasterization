#include <curitiba/render/textureCubeMap.h>
#include <curitiba/config.h>

#ifdef CURITIBA_OPENGL
#include <curitiba/render/opengl/gltextureCubeMap.h>
#endif

#include <curitiba/loader/textureloader.h>


using namespace curitiba::render;
using namespace curitiba::loader;


TextureCubeMap*
TextureCubeMap::create (std::vector<std::string> files, std::string label, bool mipmap)
{
	TextureLoader *loader[6];
	unsigned char* data[6];
	
	for (int i = 0; i < 6; i++) {
			loader[i] = TextureLoader::create();
			loader[i]->loadImage (files[i]);
			data[i] = loader[i]->getData();
	}

	std::string aFormat = loader[0]->getFormat();
	curitiba::render::TextureCubeMap *t;

#ifdef CURITIBA_OPENGL
	t = new GLTextureCubeMap (label, files, aFormat, aFormat, loader[0]->getType(), 
			loader[0]->getWidth(), data, mipmap);
#elif CURITIBA_DIRECTX
	t = new DXTexture (aDimension, aFormat, width, height);
#endif

#ifdef __SLANGER__
	int aux = loader[0]->getWidth();
	ilConvertImage(IL_RGB,IL_UNSIGNED_BYTE);
	iluScale(96,96,1);
	t->bitmap = new wxBitmap(wxImage(96,96,loader[0]->getData(),true).Mirror(false));
#endif

	for (int i = 0; i < 6; i++)
		loader[i]->freeImage();
	delete[] *loader;
	return t;
}

TextureCubeMap::TextureCubeMap(std::string label, std::vector<std::string> files, 
							   std::string internalFormat, std::string aFormat, 
							   std::string aType, int width) :
	Texture(label,"TEXTURE_CUBE_MAP", internalFormat, aFormat, aType,width,width),
	m_Files(6)
{
	for (int i = 0; i < 6; i++) 
		m_Files[i] = files[i];
}


TextureCubeMap::TextureCubeMap(std::string label, std::vector<std::string> files): 
	Texture(label),
	m_Files(6)
{
	for (int i = 0; i < 6; i++) 
		m_Files[i] = files[i];

}


TextureCubeMap::~TextureCubeMap(){

}

std::string&
TextureCubeMap::getLabel ()
{
	return m_Label;
}

void
TextureCubeMap::setLabel (std::string label)
{
	m_Label = label;
}

std::string&
TextureCubeMap::getFile (TextureCubeMapFaces i)
{
	return m_Files[i];
}

void
TextureCubeMap::setFile (std::string file, TextureCubeMapFaces i)
{
	m_Files[i] = file;
}


