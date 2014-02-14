#include <curitiba/resource/texturemanager.h>
#include <curitiba/render/textureCubeMap.h>

#include <curitiba/system/file.h>
#include <curitiba/clogger.h>

using namespace curitiba::resource;
using namespace curitiba::render;
using namespace curitiba::system;
//using namespace curitiba::material;

TextureManager::TextureManager (std::string path) : m_Path (path), m_Lib() 
{ 
}

void
TextureManager::clear() {

	while(!m_Lib.empty()) {

		delete(*m_Lib.begin());
		m_Lib.erase(m_Lib.begin());
	}
	while (!m_TexImageLib.empty()){
		
		delete((*m_TexImageLib.begin()).second);
		m_TexImageLib.erase(m_TexImageLib.begin());
	}
}

void 
TextureManager::setPath(std::string path) 
{
	m_Path = path; // Path must be empty. Setting a different path will cause trouble!!!!
}

Texture* 
TextureManager::addTexture (std::string filename, std::string label, bool mipmap) {
	
	size_t siz = m_Lib.size();
	Texture *tex;

	LOG_INFO("Adding Texture: %s to Path: %s", filename.c_str(), m_Path.c_str());

	File path (m_Path);
	File f (filename);

	if ("" == label) {
		label = path.getRelativeTo (f);
	}

	for (unsigned int i = 0; i < siz; i++) {
		tex = m_Lib[i];
		if (label == tex->getLabel()) {
			return(tex);
		}
	}

	// if the texture does not exist yet

	tex = Texture::create (filename, label, mipmap);
	if (tex)
		m_Lib.push_back(tex);

	return(tex);
}

Texture* 
TextureManager::addTexture (std::vector<std::string> filenames, std::string label, bool mipmap) {
	
	size_t siz = m_Lib.size();
	Texture *tex;

	LOG_INFO("Adding Texture: %s ", label);

	for (unsigned int i = 0; i < siz; i++) {
		tex = m_Lib[i];
		if (label == tex->getLabel()) {
			return(tex);
		}
	}

	// if the texture does not exist yet

	tex = TextureCubeMap::create (filenames, label, mipmap);
	m_Lib.push_back(tex);

	return(tex);
}
//Texture*
//TextureManager::createTexture (std::string label, 
//							   std::string internalFormat, 
//							   std::string aFormat, 
//							   std::string aType, int width, int height)
//{
//	Texture *tex;
//
//	if (true == hasTexture (label)) {
//		tex = getTexture (label); /***MARK***/ //Must check if the texture is the same (dimension, format, width, height, ...)
//		tex->setData(internalFormat,aFormat,aType,width,height);
//		return(tex);
//	}
//
//	tex = Texture::create (label, internalFormat, aFormat, aType, width, height);
//	m_Lib.push_back(tex);
//
//	return(tex);
//}


Texture*
TextureManager::createTexture (std::string label, 
							   std::string internalFormat, 
							   std::string aFormat, 
							   std::string aType, int width, int height,
							   unsigned char* data)
{
	Texture *tex;

	if (true == hasTexture (label)) {
		tex = getTexture (label); /***MARK***/ //Must check if the texture is the same (dimension, format, width, height, ...)
		tex->setData(internalFormat,aFormat,aType,width,height,data);
		return(tex);
	}

	tex = Texture::create (label, internalFormat, aFormat, aType, width, height, data);
	m_Lib.push_back(tex);

	return(tex);
}


Texture*
TextureManager::createTextureMS (std::string label, 
							   std::string internalFormat, 
							   int width, int height,
							   int samples)
{
	Texture *tex;

	if (true == hasTexture (label)) {
		tex = getTexture (label); /***MARK***/ //Must check if the texture is the same (dimension, format, width, height, ...)
		return(tex);
	}

	tex = Texture::createMS (label, internalFormat, width, height, samples);
	m_Lib.push_back(tex);

	return(tex);
}



int 
TextureManager::getTexturePosition (Texture *t) 
{

	Texture *tex;
	size_t siz = m_Lib.size();

	for (unsigned int i = 0; i < siz; i++) {
		tex = m_Lib[i];
		if (t == tex) {
			return(i);
		}
	}
	//Error - Texture Not Found!
	return(-1);
}

int 
TextureManager::getNumTextures() 
{

	return (int)m_Lib.size();
}


Texture*
TextureManager::getTexture(int id) {

	size_t siz = m_Lib.size();
	Texture *tex;

	for (unsigned int i = 0; i < siz; i++) {
		tex = m_Lib[i];
		if (id == tex->getId()) {
			return(tex);
		}
	}
	return(NULL);
}

bool
TextureManager::hasTexture(std::string &name) 
{
	size_t siz = m_Lib.size();
	Texture *tex;

	for (unsigned int i = 0; i < siz; i++) {
		tex = m_Lib[i];
		if (name == tex->getLabel()) {
			return(true);
		}
	}
	return(false);
	
}

Texture*
TextureManager::getTexture(std::string name) {

	size_t siz = m_Lib.size();
	Texture *tex;

	for (unsigned int i = 0; i < siz; i++) {
		tex = m_Lib[i];
		if (name == tex->getLabel()) {
			return(tex);
		}
	}

	tex = newEmptyTexture(name);
	m_Lib.push_back(tex);

	return(tex);
}


Texture*
TextureManager::newEmptyTexture(std::string name) {

	Texture *tex = Texture::create(name);
	m_Lib.push_back(tex);
	return(tex);
}


Texture*
TextureManager::getTextureOrdered(unsigned int position) {

	if (position < m_Lib.size()) {
		return (m_Lib[position]);
	} else {
		return NULL;
	}
}

void 
TextureManager::removeTexture (std::string name)
{
	std::vector<Texture*>::iterator texIter;

	texIter = m_Lib.begin();

	while (texIter != m_Lib.end() && (*texIter)->getLabel() != name) {
		texIter++;
	}
	if (texIter != m_Lib.end()) {
		delete (*texIter);
		m_Lib.erase (texIter);
	}
}

std::vector<std::string>*
TextureManager::getTextureLabels() {

	unsigned int i;
	std::vector<std::string> *names = new std::vector<std::string>;
	Texture *tex;

    for (i=0; i < m_Lib.size(); i++) {
		tex = m_Lib[i];
        names->push_back (tex->getLabel());   
    }
	return(names);
}


curitiba::material::TexImage* 
TextureManager::createTexImage(Texture *t) 
{
	if ( m_TexImageLib.count(t->getLabel())) {
		m_TexImageLib[t->getLabel()]->update();
		return m_TexImageLib[t->getLabel()];
	}
	
	curitiba::material::TexImage *ti = curitiba::material::TexImage::create(t);

	m_TexImageLib[t->getLabel()] = ti;

	return ti;
}

curitiba::material::TexImage* 
TextureManager::getTexImage(std::string aTextureName) 
{
	if ( m_TexImageLib.count(aTextureName)) {

		m_TexImageLib[aTextureName]->update();
		return m_TexImageLib[aTextureName];
	}
	else
		return NULL;
}

