#include <curitiba/resource/resourcemanager.h>

#include <curitiba/geometry/box.h>
#include <curitiba/geometry/sphere.h>
#include <curitiba/geometry/square.h>
#include <curitiba/geometry/axis.h>
#include <curitiba/geometry/bbox.h>
#include <curitiba/geometry/meshwithpose.h>
#include <curitiba/geometry/meshbones.h>

#include <sstream>

using namespace curitiba::resource;
using namespace curitiba::render;
using namespace curitiba::scene;
using namespace curitiba::geometry;

int ResourceManager::renderableCount = 0;

ResourceManager::ResourceManager (std::string path) :
	m_pTextureManager (0),
	m_Path (path),
	m_RenderTargets(),
	m_Meshes(),
	m_States(),
	m_Programs()
{
	m_pTextureManager = new TextureManager (m_Path);
}

ResourceManager::~ResourceManager(void)
{

	/***DESTROY ALL ALOCATED RESOURCES***/

	while (!m_States.empty()){
	
		m_States.erase(m_States.begin());
	}

	if (0 != m_pTextureManager) {
		delete m_pTextureManager;
	}

	while (!m_Meshes.empty()){
	
		m_Meshes.erase(m_Meshes.begin());
	}
	ResourceManager::renderableCount = 0;

	while (!m_RenderTargets.empty()){
	
		m_RenderTargets.erase(m_RenderTargets.begin());
	}

	while (!m_Programs.empty()){
	
		m_Programs.erase(m_Programs.begin());
	}
}

void
ResourceManager::clear() {

	m_pTextureManager->clear();

	while (!m_States.empty()){
		delete((*m_States.begin()).second);
		m_States.erase(m_States.begin());
	}

	while (!m_Meshes.empty()){
		delete((*m_Meshes.begin()).second);
		m_Meshes.erase(m_Meshes.begin());
	}

	while (!m_RenderTargets.empty()){
		delete((*m_RenderTargets.begin()).second);
		m_RenderTargets.erase(m_RenderTargets.begin());
	}

	while (!m_Programs.empty()){
		delete((*m_Programs.begin()).second);
		m_Programs.erase(m_Programs.begin());
	}
}

curitiba::material::TexImage* 
ResourceManager::createTexImage(curitiba::render::Texture *t)
{
	return m_pTextureManager->createTexImage(t);
}

curitiba::material::TexImage* 
ResourceManager::getTexImage(std::string aTextureName) 
{
	return m_pTextureManager->getTexImage(aTextureName);
}


//void 
//ResourceManager::deleteTexImage(std::string aTextureName)
//{
//	m_pTextureManager->deleteTexImage(aTextureName);
//}

bool
ResourceManager::hasTexture(std::string name)
{
	return m_pTextureManager->hasTexture(name);
}

curitiba::render::Texture*
ResourceManager::createTexture (std::string label, 
				std::string internalFormat, 
				std::string aFormat, 
				std::string aType, int width, int height,
				unsigned char* data)
{
	return (m_pTextureManager->createTexture (label, internalFormat, aFormat, aType, width, height, data));
}


curitiba::render::Texture*
ResourceManager::createTextureMS (std::string label, 
				std::string internalFormat, 
				int width, int height,
				int samples)
{
	return (m_pTextureManager->createTextureMS (label, internalFormat, width, height, samples));
}


curitiba::render::Texture* 
ResourceManager::addTexture (std::string fn, std::string label, bool mipmap)
{
	return (m_pTextureManager->addTexture (fn, label, mipmap));
}

curitiba::render::Texture* 
ResourceManager::addTexture (std::vector<std::string> fn, std::string label, bool mipmap)
{
	return (m_pTextureManager->addTexture (fn, label, mipmap));
}

curitiba::render::Texture* 
ResourceManager::newEmptyTexture(std::string &name) 
{
	return (m_pTextureManager->newEmptyTexture (name));

}

void 
ResourceManager::removeTexture (std::string name)
{
	m_pTextureManager->removeTexture (name);
}

Texture*
ResourceManager::getTexture (std::string name)
{
	return (m_pTextureManager->getTexture (name));
}

Texture *
ResourceManager::getTexture(unsigned int i) {

	if (i < m_pTextureManager->m_Lib.size())
		return (m_pTextureManager->m_Lib[i]);
	else
		return NULL;
}

int 
ResourceManager::getNumTextures() {

	return(m_pTextureManager->m_Lib.size());
}

//curitiba::render::Texture* 
//ResourceManager::createTexture (std::string label,  
//								std::string internalFormat, 
//								std::string aFormat, 
//								std::string aType, int width, int height)
//{
//	return (m_pTextureManager->createTexture (label, internalFormat, aFormat, aType, width, height));
//}

curitiba::render::RenderTarget* 
ResourceManager::createRenderTarget (std::string name, int width, int height)
{
	if (m_RenderTargets.count (name) > 0) {
		return 0;
	}
	m_RenderTargets[name] = RenderTarget::create (name, width, height);
	return m_RenderTargets[name];
}

void 
ResourceManager::removeRenderTarget (std::string name)
{
	if (m_RenderTargets.count (name) > 0){
		delete m_RenderTargets[name];
	}
}

int 
ResourceManager::getNumRenderTargets() {

	return m_RenderTargets.size();
}


curitiba::render::RenderTarget *
ResourceManager::getRenderTarget(const std::string &name) {

	if (m_RenderTargets.count (name) > 0)
		return m_RenderTargets[name];
	else
		return 0;
}

std::vector<std::string>* 
ResourceManager::getRenderTargetNames() 
{

  std::map<std::string, curitiba::render::RenderTarget*>::iterator iter;
  std::vector<std::string> *names = new std::vector<std::string>;

  for(iter = m_RenderTargets.begin(); iter != m_RenderTargets.end(); ++iter) {
    names->push_back(iter->first);   
  }
	
  return names;
}

//-------------------------------------	

//			RENDERABLES

//-------------------------------------

using namespace curitiba::geometry;

curitiba::render::IRenderable* 
ResourceManager::createRenderable(std::string type, std::string name, std::string filename)
{
	IRenderable *r = NULL;

	if ("" == name) {
		std::stringstream z;
		z << "Unnamed";
		z << ResourceManager::renderableCount++;
		name = z.str();
	}

	if (hasRenderable(name,filename))
		return(NULL);

	if (0 == type.compare ("Mesh")) 
		r = new Mesh();
	
	else if (0 == type.compare("MeshPose")) 
		r = new MeshPose();
	
	else if ("MeshBones" == type) 
		r = new MeshBones();
	
	else if ("Box" == type) 
		r = new Box();

	else if ("Square" == type) 
		r = new Square();

	else if ("Sphere" == type) 
		r = new Sphere();

	else if ("Axis" == type)
		r = new Axis();

	else if ("BoundingBox" == type)
		r = new BBox();

	r->setName(name);
	addRenderable(r,filename);
	return r;
}



bool 
ResourceManager::hasRenderable (std::string meshName, std::string filename)
{
	std::string key (filename);

	if("" != key)
		key += "#" + meshName;
	else
		key = meshName;

	if (m_Meshes.count (key) > 0) {
		return true;
	}
	return false;
}


curitiba::render::IRenderable* 
ResourceManager::getRenderable (std::string meshName, std::string filename)
{
	std::string key (filename);

	if (key != "")
		key += "#" + meshName;
	else
		key = meshName;

	if (m_Meshes.count (key) > 0) {
		return m_Meshes[key];
	}
	return 0;
}

curitiba::render::IRenderable* 
ResourceManager::addRenderable (curitiba::render::IRenderable* aMesh, std::string filename)
{
	std::string key (filename);
	std::string meshName = aMesh->getName();

	if (key != "") { 
		key += "#" + meshName;
		aMesh->setName(key);
	}
	else
		key = meshName;

	m_Meshes[key] = aMesh;

	return aMesh;
}
 

void
ResourceManager::removeRenderable(std::string name) {

	if (m_Meshes.count(name) != 0)
		m_Meshes.erase(name);
}

//

//

//

bool 
ResourceManager::hasState (std::string stateName)
{
	if (m_States.count (stateName) > 0) {
		return true;
	}
	return false;
}

curitiba::render::IState* 
ResourceManager::getState (std::string stateName)
{
	if (m_States.count (stateName) > 0) {
		return m_States[stateName];
	}
	return 0;
}

void 
ResourceManager::addState (curitiba::render::IState* aState)
{
	m_States[aState->getName()] = aState;
}



bool 
ResourceManager::hasProgram (std::string programName)
{
	if (m_Programs.count (programName) > 0){
		return true;
	}
	return false;
}

IProgram* 
ResourceManager::getProgram (std::string programName)
{
	if (false == hasProgram (programName)) {
		m_Programs[programName] = IProgram::create();
		m_Programs[programName]->setName(programName);
	}
	return m_Programs[programName];
}

unsigned int 
ResourceManager::getNumPrograms() {

	return m_Programs.size();
}

std::vector<std::string> * 
ResourceManager::getProgramNames(){

	std::vector<std::string> *names = new std::vector<std::string>; 

	for( std::map<std::string, curitiba::render::IProgram*>::iterator iter = m_Programs.begin(); iter != m_Programs.end(); ++iter ) {
      names->push_back((*iter).first); 
    }
	return names;
}