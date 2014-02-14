#include <curitiba.h>
#include <curitiba/config.h>

// added for directory loading
#ifdef CURITIBA_PLATFORM_WIN32
#include <curitiba/system/dirent.h>
#else
#include <dirent.h>
#include <sys/types.h>
#endif
#include <curitiba/math/vec4.h>

#include <ctime>

#include <GL/glew.h>

#include <curitiba/system/file.h>
#include <curitiba/scene/scenefactory.h>
//#include <curitiba/loader/colladaloader.h>
#include <curitiba/loader/cboloader.h>
//#include <curitiba/loader/threedsloader.h>
#include <curitiba/loader/objLoader.h>
#include <curitiba/loader/ogremeshloader.h>
#include <curitiba/loader/assimploader.h>
#include <curitiba/loader/projectloader.h>
#include <curitiba/world/worldfactory.h>
#include <curitiba/event/eventFactory.h>
#include <curitiba/resource/fontmanager.h>
//#include <curitiba/render/opencl/openclContext.h>


#include <curitiba/debug/profile.h>
//#include <curitiba/debug/fonts.h>

using namespace curitiba;
using namespace curitiba::system;
using namespace curitiba::loader;
using namespace curitiba::scene;
using namespace curitiba::render;
using namespace curitiba::resource;
using namespace curitiba::world;
using namespace curitiba::material;

/*
 * Improvements:
 * - Move viewports into rendermanager
 */

static curitiba::Curitiba *gInstance = 0;

curitiba::Curitiba*
Curitiba::create (void) {
	if (0 != gInstance) {
		throw (CuritibaInstanciationError ("An instance of Curitiba already exists"));
	}
	gInstance = new Curitiba;

	return gInstance;
}

curitiba::Curitiba*
Curitiba::getInstance (void) {
	if (0 == gInstance) {
		throw (CuritibaInstanciationError ("No instance of Curitiba exists"));
	}

	return gInstance;
}

Curitiba::Curitiba() :
	m_WindowWidth (0.0f), 
	m_WindowHeight (0.0f), 
	m_vViewports(),
	m_Inited (false),
	m_Physics (false),
	loadedScenes(0),
	m_ActiveCameraName(""),
	m_Name("Curitiba"),
	m_RenderFlags(COUNT_RENDER_FLAGS),
	m_UseTangents(false),
	m_UseTriangleIDs(false),
	m_CoreProfile(false)
{
	// create a default black viewport
//	createViewport("default");
}

Curitiba::~Curitiba()
{
	clear();

}

bool 
Curitiba::init (bool context, std::string aConfigFile)
{
	bool result;

	if (true == context) {
		glewExperimental = true;
		GLenum error = glewInit();
		if (GLEW_OK != error){
			std::cout << "GLEW init error: " << glewGetErrorString(error) << std::endl;
			return false;
		}
		
//		ilInit();
		m_pRenderManager = new RenderManager;
		result = m_pRenderManager->init();
		if (!result)
			return(0);

		m_pEventManager = new EventManager;



//		Camera *aCamera = m_pRenderManager->getCamera ("default");
//		Viewport* defaultViewport = createViewport ("defaultViewport", curitiba::math::vec4 (0.0f, 0.0f, 0.0f, 1.0f));
//		aCamera->setViewport (defaultViewport);
		
	}	
	
	m_pResourceManager = new ResourceManager ("."); /***MARK***/ //Get path!!!
	m_pMaterialLibManager = new MaterialLibManager();

	try {
		ProjectLoader::loadMatLib("./curitibaSystem.mlib");
	}
	catch (std::string s) {
		clear();
		throw(s);
	}

	FontManager::addFont("CourierNew10", "./couriernew10.xml", "__FontCourierNew10");

//	m_ProfileMaterial =	MATERIALLIBMANAGER->getDefaultMaterial("__Emission White");


	//m_pScene = SceneFactory::create ("Octree"); /***MARK***/ //Check for 0. Configuration
	m_pWorld = WorldFactory::create ("Bullet");

//	m_pWorld->setScene (m_pScene);
	
//	m_pConsole = m_pRenderManager->createConsole (createViewport());

	CLOCKS_PER_MILISEC = CLOCKS_PER_SEC / 1000.0;
	INV_CLOCKS_PER_MILISEC = 1.0 / CLOCKS_PER_MILISEC;

	m_CurrentTime = clock() * INV_CLOCKS_PER_MILISEC;
	m_LastFrameTime = NO_TIME;

//	Profile::init();

	m_Inited = true;

	EVENTMANAGER->addListener("WINDOW_SIZE_CHANGED",this);

	m_DefaultState = IState::create();
	m_Viewport = createViewport("defaultFixedVP");

	return true;
}

std::string&
Curitiba::getName() 
{
	return(m_Name);
}

void
Curitiba::eventReceived(const std::string &sender, const std::string &eventType, IEventData *evtData) 
{
	if (eventType == "WINDOW_SIZE_CHANGED") {
	
		vec3 *evVec = (vec3 *)evtData->getData();
		setWindowSize(evVec->x,evVec->y);
	}
}

void
Curitiba::readProjectFile (std::string file, int *width, int *height)
{
	//clear();

	try {
		ProjectLoader::load (file, width, height, &m_UseTangents, &m_UseTriangleIDs);
	}
	catch (std::string s) {
		clear();
		throw(s);
	}
		/* Situations like this should be replaced in a near future with a events patern*/
	//std::vector<std::string> *materialNames = MATERIALLIBMANAGER->getMaterialNames (DEFAULTMATERIALLIBNAME);
	//RENDERMANAGER->materialNamesFromLoadedScenes (*materialNames); 
	//delete materialNames;	

	setActiveCameraName(RENDERMANAGER->getDefaultCameraName());
		
	m_pWorld->setScene(RENDERMANAGER->getScene("Terrain"));

	if (m_UseTriangleIDs)
		RENDERMANAGER->prepareTriangleIDs(true);
}

void
Curitiba::readDirectory (std::string dirName)
{
	DIR *dir;
	struct dirent *ent;
	bool result = true;
	char fileName [1024];
	char sceneName[256];

	clear();
	sprintf(sceneName,"MainScene"); //,loadedScenes);
	loadedScenes++;
	RENDERMANAGER->createScene (sceneName);
	dir = opendir (dirName.c_str());

	if (0 == dir) {
		CURITIBA_THROW("Can't open dir: %s",dirName);
	}
	while (0 != (ent = readdir (dir))) {

#ifdef CURITIBA_PLATFORM_WIN32
		sprintf (fileName, "%s\\%s", dirName.c_str(), ent->d_name);
#else
		sprintf (fileName, "%s/%s", dirName, ent->d_name);						
#endif
		try {
			CURITIBA->loadAsset (fileName, sceneName);
		}
		catch(std::string &s) {
			closedir(dir);
			throw(s);
		}
	}
	closedir (dir);
	loadFilesAndFoldersAux(sceneName,false);	
	
}

void
Curitiba::clear() {

	setActiveCameraName("");
	SceneObject::ResetCounter();
	MATERIALLIBMANAGER->clear();
	EVENTMANAGER->clear(); 
	EVENTMANAGER->addListener("WINDOW_SIZE_CHANGED",this);
	RENDERMANAGER->clear();
	RESOURCEMANAGER->clear();
	
	// Need to clear font manager

	while (!m_vViewports.empty()){
	
		m_vViewports.erase(m_vViewports.begin());
	}

	m_Viewport = createViewport("defaultFixedVP");

	//_CrtDumpMemoryLeaks();

	ProjectLoader::loadMatLib("./curitibaSystem.mlib");

}

void
Curitiba::readModel (std::string fileName) throw (std::string)
{
	clear();
	bool result = true;
	
	char sceneName[256];

	sprintf(sceneName,"MainScene"); //,loadedScenes);
	loadedScenes++;

	RENDERMANAGER->createScene (sceneName);

	try {
		CURITIBA->loadAsset (fileName, sceneName);
		loadFilesAndFoldersAux(sceneName, false);
	} 
	catch (std::string &s) {
			throw(s);
	}
}


void
Curitiba::appendModel(std::string fileName)

{
	
	char sceneName[256];

	sprintf(sceneName,"MainScene"); //,loadedScenes);
	loadedScenes++;

	RENDERMANAGER->createScene (sceneName);

	try {
		CURITIBA->loadAsset (fileName, sceneName);
		loadFilesAndFoldersAux(sceneName, false);
	}
	catch( std::string &s){
		throw(s);
	}
}


void Curitiba::loadFilesAndFoldersAux(char *sceneName, bool unitize) {

	Camera *aNewCam = RENDERMANAGER->getCamera ("MainCamera");
	Viewport *v = CURITIBA->createViewport ("MainViewport", curitiba::math::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	aNewCam->setViewport (v);

	setActiveCameraName("MainCamera");

	if (unitize) {
		aNewCam->setPerspective (60.0f, 0.01f, 100.0f);
		aNewCam->setProp(Camera::POSITION, 0.0f, 0.0f, 5.0f, 1.0f);
	}
	else
		aNewCam->setPerspective (60.0f, 1.0f, 10000.0f);

	// creates a directional light by default
	Light *l = RENDERMANAGER->getLight ("MainDirectionalLight");
	l->setProp(Light::DIRECTION,1.0f,-1.0f,-1.0f, 0.0f);
	l->setProp(Light::DIFFUSE, 0.9f,0.9f,0.9f,1.0f);
	l->setProp(Light::AMBIENT,0.5f,0.5f,0.5f,1.0f );

	Pipeline *aPipeline = RENDERMANAGER->getPipeline ("MainPipeline");
	Pass *aPass = aPipeline->createPass("MainPass");
	aPass->setCamera ("MainCamera");

	aPass->setViewport (v);
	aPass->setProp(IRenderer::COLOR_CLEAR, true);
	aPass->setProp(IRenderer::DEPTH_CLEAR, true);
	aPass->addLight ("MainDirectionalLight");

	aPass->addScene(sceneName);
	RENDERMANAGER->setActivePipeline ("MainPipeline");

	//std::vector<std::string> *materialNames = MATERIALLIBMANAGER->getMaterialNames (DEFAULTMATERIALLIBNAME);
	//RENDERMANAGER->materialNamesFromLoadedScenes (*materialNames); 
	//delete materialNames;

//	RENDERMANAGER->prepareTriangleIDsAndTangents(true,true);

}

bool
Curitiba::reload (void)
{
	if (false == m_Inited) {
		return false;
	}
	//RenderManager->reload();

	return true;
}

//void 
//curitiba::Curitiba::setProfileMaterial(std::string aMaterial) {
//
//	m_ProfileMaterial = MATERIALLIBMANAGER->getDefaultMaterial(aMaterial);
//	if (m_ProfileMaterial == 0)
//		m_ProfileMaterial = MATERIALLIBMANAGER->getDefaultMaterial("__Emission White");
//}


void
Curitiba::step (void)
{
	IRenderer *renderer = RENDERER;
	m_CurrentTime = clock() * INV_CLOCKS_PER_MILISEC;
	if (NO_TIME == m_LastFrameTime) {
		m_LastFrameTime = m_CurrentTime;
	}
	double deltaT = m_CurrentTime - m_LastFrameTime;
	m_LastFrameTime = m_CurrentTime;

	m_pEventManager->notifyEvent("FRAME_BEGIN","Curitiba", "", NULL);

	renderer->resetCounters();

	//if (m_Animations.size() > 0) {
	//	std::map<std::string, curitiba::animation::IAnimation*>::iterator animIter;
	//	
	//	animIter = m_Animations.begin();

	//	for (; animIter != m_Animations.end(); animIter++) {
	//		if (false == (*animIter).second->isFinished()) {
	//			(*animIter).second->step (static_cast<float> (deltaT));
	//		} else {
	//			delete ((*animIter).second);
	//			m_Animations.erase (animIter);
	//		}
	//	}
	//}

	if (true == m_Physics) {
		m_pWorld->update();	
		m_pEventManager->notifyEvent("DYNAMIC_CAMERA", "MainCanvas", "", NULL);
	}

	//renderer->setDefaultState();

	m_pRenderManager->renderActivePipeline();

#ifdef CURITIBA_RENDER_FLAGS
//#ifdef PROFILE
//	if (getRenderFlag(Curitiba::PROFILE_RENDER_FLAG))
//	{
//		PROFILE ("Profile rendering");
//
//		renderer->setViewport(m_WindowWidth, m_WindowHeight);
//
//		renderer->saveAttrib(IRenderer::RENDER_MODE);
//		renderer->setRenderMode(IRenderer::MATERIAL_MODE);
//		
//		m_ProfileMaterial->prepare();
//		renderer->disableDepthTest();
//		//RENDERER->enableTexturing();
//		setOrthographicProjection (static_cast<int>(m_WindowWidth), 
//										static_cast<int>(m_WindowHeight));
//
//		Profile::dumpLevelsOGL();
//
// 		char s[128];
// 		sprintf (s, "Primitives: %d", RENDERER->getTriCount());
// 		renderBitmapString (30,400, Profile::font,s);
//		
//		resetPerspectiveProjection();
//		renderer->enableDepthTest();
//		renderer->restoreAttrib();
//	}
//#endif // PROFILE
#endif // CURITIBA_RENDER_FLAGS

	m_pEventManager->notifyEvent("FRAME_END","Curitiba", "", NULL);
}


void 
Curitiba::setActiveCameraName(const std::string &aCamName) {

	if (m_ActiveCameraName != "") {
		EVENTMANAGER->removeListener("CAMERA_MOTION",RENDERMANAGER->getCamera(m_ActiveCameraName));
		EVENTMANAGER->removeListener("CAMERA_ORIENTATION",RENDERMANAGER->getCamera(m_ActiveCameraName));
	}
	m_ActiveCameraName = aCamName;

	if (m_ActiveCameraName != "") {
		EVENTMANAGER->addListener("CAMERA_MOTION",RENDERMANAGER->getCamera(m_ActiveCameraName));
		EVENTMANAGER->addListener("CAMERA_ORIENTATION",RENDERMANAGER->getCamera(m_ActiveCameraName));
	}
}


curitiba::scene::Camera *
Curitiba::getActiveCamera() {

	if (RENDERMANAGER->hasCamera(m_ActiveCameraName)) {
		return RENDERMANAGER->getCamera(m_ActiveCameraName);
	}
	else
		return NULL;
}

void 
Curitiba::sendKeyToEngine (char keyCode)
{
	if ('K' == keyCode) {
		Profile::Reset();
	}

	//RenderManager->sendKeyToEngine (keyCode);
}



IWorld&
Curitiba::getWorld (void)
{
	return (*m_pWorld);
}



void
Curitiba::loadAsset (std::string aFilename, std::string sceneName, std::string params) throw (std::string)
{
	File file (aFilename);

	try {
		switch (file.getType()) {
			case File::COLLADA:
			{
				AssimpLoader::loadScene(RENDERMANAGER->getScene (sceneName), file.getFullPath());
				//std::string uri (file.getURI());
				//ColladaLoader::loadScene (RENDERMANAGER->getScene (sceneName), uri);
				break;
			}
			case File::CURITIBABINARYOBJECT:
			{
				std::string filepath (file.getFullPath());
				CBOLoader::loadScene (RENDERMANAGER->getScene (sceneName), filepath);
				break;
			}
			case File::THREEDS:
			{
				AssimpLoader::loadScene(RENDERMANAGER->getScene (sceneName), file.getFullPath(), params);
				//THREEDSLoader::loadScene (RENDERMANAGER->getScene (sceneName), file.getFullPath(),params);
				
				break;
			}
			case File::WAVEFRONTOBJ:
			{
				//AssimpLoader::loadScene(RENDERMANAGER->getScene (sceneName), file.getFullPath());
				OBJLoader::loadScene(RENDERMANAGER->getScene (sceneName), file.getFullPath());
				
				break;
			}
			case File::OGREXMLMESH:
			{
				OgreMeshLoader::loadScene(RENDERMANAGER->getScene (sceneName), file.getFullPath());
				
				break;
			}

			default:
			  break;
		}
	}
	catch(std::string &s) {
		throw(s);
	}

	Profile::Reset();
}

void
Curitiba::writeAssets (std::string fileType, std::string aFilename, std::string sceneName)
{
	if (0 == fileType.compare ("CBO")) {
		CBOLoader::writeScene (RENDERMANAGER->getScene (sceneName), aFilename);
	}
}

void
Curitiba::setWindowSize (float width, float height)
{
	m_Viewport->setSize(width,height);
	m_WindowWidth = width;
	m_WindowHeight = height;
}

float 
Curitiba::getWindowHeight() 
{
	return(m_WindowHeight);
}

float 
Curitiba::getWindowWidth() 
{
	return(m_WindowWidth);
}



Viewport*
Curitiba::createViewport (const std::string &name, const curitiba::math::vec4 &bgColor) 
{
	Viewport* v = new Viewport;

	v->setName(name);
	v->setOrigin (0,0);
	v->setSize (m_WindowWidth, m_WindowHeight);

	v->setBackgroundColor (bgColor);

	m_vViewports[name] = v;

	return v;
}

Viewport*
Curitiba::createViewport (const std::string &name) 
{
	Viewport* v = new Viewport;

	v->setName(name);
	v->setOrigin (0,0);
	v->setSize (m_WindowWidth, m_WindowHeight);

	m_vViewports[name] = v;

	return v;
}


Viewport* 
Curitiba::getViewport (const std::string &name)
{
	if (m_vViewports.count(name))
		return m_vViewports[name];
	else
		return NULL;
}



Viewport*
Curitiba::getDefaultViewport() {
	
	return m_Viewport;
}


std::vector<std::string> *
Curitiba::getViewportNames()
{
	std::vector<std::string> *names = new std::vector<std::string>; 

	for( std::map<std::string, curitiba::render::Viewport*>::iterator iter = m_vViewports.begin(); iter != m_vViewports.end(); ++iter ) {
      names->push_back(iter->first); 
    }
	return names;
}

void
Curitiba::enablePhysics (void) 
{
	m_Physics = true;
}

void
Curitiba::disablePhysics (void)
{
	m_Physics = false;
}


void
Curitiba::setRenderFlag(RenderFlags aFlag, bool aState) 
{
	m_RenderFlags[aFlag] = aState;
}


bool
Curitiba::getRenderFlag(RenderFlags aFlag)
{
	return(m_RenderFlags[aFlag]);
}

int 
Curitiba::picking (int x, int y, std::vector<curitiba::scene::SceneObject*> &objects, curitiba::scene::Camera &aCamera)
{
	return -1;//	RenderManager->pick (x, y, objects, aCamera);
}

//void 
//Curitiba::addAnimation (std::string animationName, curitiba::animation::IAnimation *anAnimation)
//{
//	m_Animations [animationName] = anAnimation; //Attention! Possibility of memory leak
//}
//
//curitiba::animation::IAnimation*
//Curitiba::getAnimation (std::string animationName)
//{
//	if (m_Animations.count (animationName) > 0) {
//		return m_Animations [animationName];
//	}
//	
//	return 0;
//}

//void 
//Curitiba::enableStereo (void)
//{
//	RENDERMANAGER->enableStereo();
//}
//
//void 
//Curitiba::disableStereo (void)
//{
//	RENDERMANAGER->disableStereo();
//}

RenderManager* 
Curitiba::getRenderManager (void)
{
	return m_pRenderManager;
}

ResourceManager* 
Curitiba::getResourceManager (void)
{
	return m_pResourceManager;
}

MaterialLibManager*
Curitiba::getMaterialLibManager (void)
{
	return m_pMaterialLibManager;
}

EventManager*
Curitiba::getEventManager (void)
{
	return m_pEventManager;
}


/* =================================================

	DATA TYPES

================================================= */

std::string 
Curitiba::getDataType(DataType dt) 
{
	switch(dt) {
		case INT: return("INT");
		case SAMPLER: return("SAMPLER");
		case BOOL: return("BOOL");
		case IVEC2: return("IVEC2");
		case IVEC3: return("IVEC3");
		case IVEC4: return("IVEC4");
		case BVEC2: return("BVEC2");
		case BVEC3: return("BVEC3");
		case BVEC4: return("BVEC4");
		case FLOAT: return("FLOAT");
		case VEC2: return("VEC2");
		case VEC3: return("VEC3");
		case VEC4: return("VEC4");
		case MAT3: return("MAT3");
		case MAT4: return("MAT4");
		default: return ("FLOAT");
	}
}

Curitiba::DataType 
Curitiba::getDataType(std::string &s)
{
	if ("INT" == s)
		return INT;
	else if ("SAMPLER" == s)
		return SAMPLER;
	else if ("BOOL" == s)
		return BOOL;
	else if ("IVEC2" == s)
		return IVEC2;
	else if ("IVEC3" == s)
		return IVEC3;
	else if ("IVEC4" == s)
		return IVEC4;
	else if ("BVEC2" == s)
		return BVEC2;
	else if ("BVEC3" == s)
		return BVEC3;
	else if ("BVEC4" == s)
		return BVEC4;
	else if ("FLOAT" == s)
		return FLOAT;
	else if ("VEC2" == s)
		return VEC2;
	else if ("VEC3" == s)
		return VEC3;
	else if ("VEC4" == s)
		return VEC4;
	else if ("MAT3" == s)
		return MAT3;
	else if ("MAT4" == s)
		return MAT4;
	else
		return FLOAT;
}
