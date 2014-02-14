#include <curitiba/render/pass.h>
#include <curitiba/geometry/axis.h>

#include <sstream>
#include <algorithm>

#include <curitiba.h>
#include <curitiba/geometry/frustum.h>
#include <curitiba/debug/profile.h>

using namespace curitiba::material;
using namespace curitiba::scene;
using namespace curitiba::render;
using namespace curitiba::geometry;


Pass::Pass (const std::string &passName) :
	m_ClassName("default"),
	m_Name (passName),
	m_CameraName ("default"),
	m_SceneVector(),
	m_MaterialMap(),
	m_pViewport (0),
	m_pRestoreViewport (0),
	m_RemapMode (REMAP_DISABLED),
	m_Paramf(),
	m_Params(),
	m_ParamType(),
	m_BoolProp(IRenderer::COUNT_BOOL_PROPS)
{
	initVars();
	EVENTMANAGER->addListener("SCENE_CHANGED",this);
}


void
Pass::eventReceived(const std::string &sender, const std::string &eventType, IEventData *evtData) 
{
	if (eventType == "SCENE_CHANGED") 
		updateMaterialMaps(sender);
}


const std::string &
Pass::getClassName() {

	return m_ClassName;
}


const std::map<std::string, float> &
Pass::getParamsf() 
{
	return m_Paramf;
}


void 
Pass::setParam(const std::string &name, const float value) 
{
	if (m_ParamType.count(name) && m_ParamType[name] == Pass::FLOAT)
		m_Paramf[name] = value;
}


void
Pass::setParam(const std::string &name, const std::string &value) 
{
	if (m_ParamType.count(name) && m_ParamType[name] == Pass::STRING)
		m_Params[name] = value;
}


float *
Pass::getParamf(const std::string &name)
{
	if (m_Paramf.count(name))
		return (&m_Paramf[name]);
	else
		return NULL;

}

std::string &
Pass::getParams(const std::string &name)
{
	if (m_Paramf.count(name))
		return(m_Params[name]);
	else
		return p_Empty;
}


int 
Pass::getParamType(const std::string &name) {

	if (m_ParamType.count(name))
		return m_ParamType[name];
	else
		return NOT_DEFINED;
}


std::string &
Pass::getName (void)
{
	return m_Name;
}


void 
Pass::setViewport (curitiba::render::Viewport *aViewport)
{
	m_pViewport = aViewport;
}


curitiba::render::Viewport *
Pass::getViewport()
{
	return (m_pViewport);
}


Pass::~Pass()
{
	m_pViewport = NULL;
	/***MARK***/ //Delete resources ?
}


void 
Pass::initVars() {

	m_BoolProp[IRenderer::COLOR_CLEAR] = true;
	m_BoolProp[IRenderer::COLOR_ENABLE] = true;

	m_BoolProp[IRenderer::DEPTH_CLEAR] = true;
	m_BoolProp[IRenderer::DEPTH_ENABLE] = true;
	m_BoolProp[IRenderer::DEPTH_MASK] = true;

	m_BoolProp[IRenderer::STENCIL_ENABLE] = false;
	m_BoolProp[IRenderer::STENCIL_CLEAR] = true;

	m_RenderTarget = 0;

	m_ColorClearValue = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	m_DepthClearValue = 1.0f;
	m_DepthFunc = IState::LESS;
	m_StencilClearValue = 0.0f;

	//for (int i = 0; i < MAXFBOs + 1; i++)
	//	m_TexId[i] = 0 ;

	m_Color = 0; // how many color render targets
	m_Depth = 0; // render depth?

	m_RTSizeWidth = 512; // size of render targets
	m_RTSizeHeight = 512;

	m_UseRT = false; // enable, disable
}


void 
Pass::setRTSize (int width, int height) 
{
	assert(m_pViewport != NULL);

	m_RTSizeWidth = width;
	m_RTSizeHeight = height;
	m_pViewport->setSize(width,height);
	m_pViewport->setOrigin(0.0f, 0.0f);
	m_pViewport->setFixed(true);
}


void
Pass::prepareBuffers() {

	int clear = 0;

	if (m_BoolProp[IRenderer::DEPTH_CLEAR]) {
		clear = IRenderer::DEPTH_BUFFER;
		RENDERER->setDepthClearValue(m_DepthClearValue);
	}
	if (m_BoolProp[IRenderer::COLOR_CLEAR]) {
		clear |= IRenderer::COLOR_BUFFER;
	}
	if (m_BoolProp[IRenderer::STENCIL_CLEAR]) {
		RENDERER->setStencilClearValue(m_StencilClearValue);
		clear |= IRenderer::STENCIL_BUFFER;
	}

	RENDERER->clear (clear);

	if (m_BoolProp[IRenderer::DEPTH_ENABLE]) {
		RENDERER->setProp(IRenderer::DEPTH_ENABLE,true);
		RENDERER->setProp(IRenderer::DEPTH_MASK, m_BoolProp[IRenderer::DEPTH_MASK]);
		RENDERER->setDepthFunc(m_DepthFunc);
	}
	else
		RENDERER->setProp(IRenderer::DEPTH_ENABLE,false);

	if (m_BoolProp[IRenderer::STENCIL_ENABLE]) {
		RENDERER->setProp(IRenderer::STENCIL_ENABLE, true);
		RENDERER->setStencilFunc(m_StencilFunc, m_StencilOpRef, m_StencilOpMask);
		RENDERER->setStencilOp(m_Stencilsfail, m_Stencildfail, m_Stencildpass);
	}
	else {
		RENDERER->setProp(IRenderer::STENCIL_ENABLE, false);
	}
}


void
Pass::prepare (void)
{
	if (0 != m_RenderTarget && true == m_UseRT) {
		m_RenderTarget->bind();
	}

	setupCamera();

	prepareBuffers();

	setupLights();
}


void
Pass::restore (void)
{
	if (0 != m_RenderTarget && true == m_UseRT) {
		m_RenderTarget->unbind();
	}

	restoreCamera();
	RENDERER->removeLights();
}


bool 
Pass::renderTest (void)
{
	return true;
}


void
Pass::doPass (void)
{
	Camera *aCam = 0;
	Frustum camFrustum;
	std::vector<SceneObject*>::iterator objsIter;
	std::vector<std::string>::iterator scenesIter;
	std::vector<curitiba::scene::SceneObject*> sceneObjects;

	const float *a = RENDERER->getMatrix(IRenderer::PROJECTION_VIEW_MODEL);
	//const float *b = RENDERER->getProjectionModelviewMatrix();
	camFrustum.setFromMatrix (a);
	aCam = RENDERMANAGER->getCamera (m_CameraName);
	RENDERMANAGER->clearQueue();

	//RENDERER->setProp(IRenderer::DEPTH_ENABLE, true);

#if (CURITIBA_CORE_OPENGL == 0)
	RENDERER->enableTexturing();
	if (m_Lights.size()) {
		RENDERER->activateLighting();
	}
#endif

	scenesIter = m_SceneVector.begin();

	for ( ; scenesIter != m_SceneVector.end(); ++scenesIter) {

		IScene *aScene = RENDERMANAGER->getScene (*scenesIter);
		{
			PROFILE("View Frustum Culling");
			sceneObjects = aScene->findVisibleSceneObjects(camFrustum, *aCam);
		//	sceneObjects = aScene->getAllObjects();
		}
		objsIter = sceneObjects.begin();
		for ( ; objsIter != sceneObjects.end(); ++objsIter) {
			RENDERMANAGER->addToQueue ((*objsIter), m_MaterialMap);
		}
	}


	RENDERMANAGER->processQueue();	

//	RENDERER->disableDepthTest();

#if (CURITIBA_CORE_OPENGL == 0)
	RENDERER->disableTexturing();
	RENDERER->deactivateLighting();
#endif
}


void
Pass::addLight (const std::string &name)
{
	assert(!hasLight(name));

	m_Lights.push_back (name);
}


void
Pass::removeLight(const std::string &name)
{
	std::vector <std::string>::iterator iter;

	iter = m_Lights.begin();
	while (iter != m_Lights.end() && (*iter) != name) {
		++iter;
	}
	if (iter != m_Lights.end())
		m_Lights.erase(iter);
}


bool
Pass::hasLight(const std::string &name) 
{
	std::vector <std::string>::iterator iter;

	iter = m_Lights.begin();
	while (iter != m_Lights.end() && (*iter) != name) {
		++iter;
	}
	if (iter != m_Lights.end())
		return true;
	else
		return false;
}


const std::map<std::string, curitiba::material::MaterialID> &
Pass::getMaterialMap() 
{
	return m_MaterialMap;
}


void 
Pass::remapMaterial (const std::string &originMaterialName, const std::string &materialLib, const std::string &destinyMaterialName)
{
	m_MaterialMap[originMaterialName].setMaterialID (materialLib, destinyMaterialName);
}


void 
Pass::remapAll (const std::string &materialLib, const std::string &destinyMaterialName)
{
	m_RemapMode = REMAP_TO_ONE;
	m_MaterialMap["*"].setMaterialID (materialLib, destinyMaterialName);

	std::map<std::string, MaterialID>::iterator matMapIter;
	
	matMapIter = m_MaterialMap.begin();

	for ( ; matMapIter != m_MaterialMap.end(); ++matMapIter) {
		(*matMapIter).second.setMaterialID (materialLib, destinyMaterialName);
	}
}


void
Pass::remapAll (const std::string &targetLibrary)
{
	m_RemapMode = REMAP_TO_LIBRARY;
	m_MaterialMap["*"].setMaterialID (targetLibrary, "*");

	std::map<std::string, MaterialID>::iterator matMapIter;
  
	matMapIter = m_MaterialMap.begin();
  
	for (; matMapIter != m_MaterialMap.end(); ++matMapIter) {
		if (MATERIALLIBMANAGER->hasMaterial(targetLibrary, (*matMapIter).first))
			(*matMapIter).second.setMaterialID (targetLibrary, (*matMapIter).first);
	}
}




void 
Pass::materialNamesFromLoadedScenes (std::vector<std::string> &materials)
{
	std::vector<std::string>::iterator matIter;

	matIter = materials.begin();
	
	for ( ; matIter != materials.end(); ++matIter) {

		if (0 == m_MaterialMap.count (*matIter)) {

			switch (m_RemapMode) {
		  
				case REMAP_TO_ONE:
					m_MaterialMap[*matIter].setMaterialID (m_MaterialMap["*"].getLibName(), m_MaterialMap["*"].getMaterialName());
					break;

				case REMAP_TO_LIBRARY:
					m_MaterialMap[*matIter].setMaterialID (m_MaterialMap["*"].getLibName(), *matIter);
					break;

				default:
					m_MaterialMap[*matIter].setMaterialID (DEFAULTMATERIALLIBNAME, *matIter);
			}
		}
	}
}


bool
Pass::hasRenderTarget() 
{
	return (m_RenderTarget != 0);
}

 
curitiba::render::RenderTarget* 
Pass::getRenderTarget (void)
{
	return m_RenderTarget;
}


bool 
Pass::isRenderTargetEnabled() 
{
	return m_UseRT;
}


void 
Pass::enableRenderTarget(bool b)
{
	m_UseRT = b;
}


void 
Pass::setRenderTarget (curitiba::render::RenderTarget* rt)
{

	if (rt == NULL) {
		if (m_RenderTarget != NULL) 
			delete m_pViewport;
		m_UseRT = true;
	}
	else {
		if (m_RenderTarget == NULL){
			m_pViewport = new Viewport();
			m_UseRT = true;
		}
		setRTSize(rt->getWidth(), rt->getHeight());
		m_pViewport->setBackgroundColor(rt->getClearValues());
	}
	m_RenderTarget = rt;
}


void
Pass::setupCamera (void)
{
	Camera *aCam = 0;

	aCam = RENDERMANAGER->getCamera (m_CameraName);
	
	if (0 == aCam) {
		return; 
	}

	if (0 != m_pViewport) {
		m_pRestoreViewport = aCam->getViewport();
		aCam->setViewport (m_pViewport);
	}
	
	RENDERER->setCamera (aCam);
}


void
Pass::restoreCamera (void)
{
	Camera *aCam = 0;

	aCam = RENDERMANAGER->getCamera (m_CameraName);

	if (0 == aCam) {
		return; 
	}
	
	if (0 != m_pViewport) {
		aCam->setViewport (m_pRestoreViewport);
	}
}


void
Pass::setupLights (void)
{
	std::vector<std::string>::iterator lightsIter;

	lightsIter = m_Lights.begin();

	for (; lightsIter != m_Lights.end(); ++lightsIter) {

		Light *l = RENDERMANAGER->getLight (*lightsIter);

#ifdef COMPATIBILITY_FLAG_OPENGL
		if (l->getPropb(Light::ENABLED)) {
			RENDERER->activateLighting();
		}
#endif
		RENDERER->addLight (*l);
#ifdef COMPATIBILITY_FLAG_OPENGL
		RENDERER->positionLight (*l);
#endif
	}
}


const std::string& 
Pass::getCameraName (void)
{
	return m_CameraName;
}


void 
Pass::setCamera (const std::string &cameraName)
{
	m_CameraName = cameraName;
}



void 
Pass::setProp(IRenderer::BoolProps prop, bool value) {

	m_BoolProp[prop] = value;
}


bool
Pass::getPropb(IRenderer::BoolProps prop) {

	return m_BoolProp[prop];
}



void 
Pass::setStencilFunc(IState::Func f, int ref, unsigned int mask) {

	m_StencilFunc = f;
	m_StencilOpRef = ref;
	m_StencilOpMask = mask;
}


void 
Pass::setStencilOp(	IRenderer::StencilOp sfail, 
							IRenderer::StencilOp dfail, 
							IRenderer::StencilOp dpass) {

	m_Stencilsfail = sfail;
	m_Stencildfail = dfail;
	m_Stencildpass = dpass;
}


//void
//Pass::setDoColorClear (bool value)
//{
//	m_ = value;
//}


//bool
//Pass::getDoColorClear (void)
//{
//	return m_DoColorClear;
//}


//void
//Pass::setDoDepthClear (bool value)
//{
//	m_DoDepthClear = value;
//}


//bool
//Pass::getDoDepthClear (void)
//{
//	return m_DoDepthClear;
//}


void 
Pass::setDepthClearValue(float v) {

	m_DepthClearValue = v;
}


void 
Pass::setDepthFunc(IState::Func f) {

	m_DepthFunc = f;
}



//void 
//Pass::setDepthMask(bool b) {
//
//	m_DepthMask = b;
//}


//void
//Pass::setDoStencilClear (bool value)
//{
//	m_DoStencilClear = value;
//}


void
Pass::setStencilMaskValue (int value)
{
	m_StencilMaskValue = value;
}


//bool
//Pass::getDoStencilClear (void)
//{
//	return m_DoStencilClear;
//}


void 
Pass::setStencilClearValue(float v) {

	m_StencilClearValue = v;
}


void 
Pass::updateMaterialMaps(const std::string &sceneName) 
{
	if (this->hasScene(sceneName)) {
		std::set<std::string> *materialNames = new std::set<std::string>;
		RENDERMANAGER->getScene(sceneName)->getMaterialNames(materialNames);
		
		std::set<std::string>::iterator iter;
		iter = materialNames->begin();
		for ( ; iter != materialNames->end(); ++iter ) {
			
			if (m_MaterialMap.count((*iter)) == 0)
				m_MaterialMap[(*iter)] = MaterialID(DEFAULTMATERIALLIBNAME, (*iter));
		}
		delete materialNames;
	}
}


void 
Pass::addScene (const std::string &sceneName)
{
	if (m_SceneVector.end() == std::find (m_SceneVector.begin(), m_SceneVector.end(), sceneName)) {
	
		m_SceneVector.push_back (sceneName);
	
		std::set<std::string> *materialNames = new std::set<std::string>;
		RENDERMANAGER->getScene(sceneName)->getMaterialNames(materialNames);
		
		std::set<std::string>::iterator iter;
		iter = materialNames->begin();
		for ( ; iter != materialNames->end(); ++iter) {
			
			if (m_MaterialMap.count((*iter)) == 0)
				m_MaterialMap[(*iter)] = MaterialID(DEFAULTMATERIALLIBNAME, (*iter));
		}
		delete materialNames;
	}
//	RENDERMANAGER->getScene(sceneName)->compile();

}


void
Pass::removeScene(const std::string &name)
{
	std::vector <std::string>::iterator iter;

	iter = m_SceneVector.begin();
	while (iter != m_SceneVector.end() && (*iter) != name) {
		++iter;
	}
	if (iter != m_SceneVector.end())
		m_SceneVector.erase(iter);
}


bool
Pass::hasScene(const std::string &name) 
{
	std::vector <std::string>::iterator iter;

	iter = m_SceneVector.begin();
	while (iter != m_SceneVector.end() && (*iter) != name) {
		++iter;
	}
	if (iter != m_SceneVector.end())
		return true;
	else
		return false;
}


const std::vector<std::string>& 
Pass::getScenesNames (void)
{
	return m_SceneVector;
}


/*
---------------------------------------------- CUT HERE (OLD CODE) ----------------------------------------------------------
*/

//void 
//Pass::setCamera (curitiba::scene::Camera *aCam)
//{
//	m_PassCamera = aCam;
//}

//void 
//Pass::setScene (curitiba::scene::IScene *aScene)
//{
//	m_Scene = aScene;
//}


//void 
//Pass::setMaterialLib (std::string materialLib)
//{
//	m_MaterialLib = materialLib;
//}

//void 
//Pass::setSceneObjectsInFrustum (std::vector<curitiba::scene::ISceneObject*> &sceneObjects)
//{
//	m_SceneObjects = sceneObjects;
//}

// This render the entire model 
//void 
//Pass::render()
//{
//
//	if (m_fbColor && m_useRT) 
//		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbColor);
//	else if (m_fbDepth && m_useRT)
//		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbDepth);
///*	else
//		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
//*/
//	// clear buffers
//	int clear = 0;
//	if (m_clearColor)
//		clear = GL_COLOR_BUFFER_BIT;
//	if (m_clearDepth)
//		clear = clear | GL_DEPTH_BUFFER_BIT;
//	glClear( clear  );
//
//	if (m_model != NULL)
//		//m_model->render(m_camera,m_projection);
//		m_model->render();
//
//	if (m_fbColor && m_useRT) 
//		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
//	if (m_fbDepth && m_useRT)
//		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
//}

//void CPass::save(std::ofstream &outf, std::string path) {
//
//
//	std::string fullname = m_model->path;
//	fullname += "/";
//	fullname += m_model->name;
//	std::string rel = CFilename::GetRelativeFileName(path,fullname);
//	if (rel == m_model->name)
//		rel = "./";
//	outf << "\tMODEL NAME=" << m_model->name << "\n";
//	outf << "\tMODEL PATH=" << rel << "\n";
//	outf << "\tRT SIZE=" << m_rtSize << "\n";
//	outf << "\tNUM RT=" << m_color << "\n";
//	outf << "\tCAMERA ATTACHMENT =" << m_cameraAttachment << "\n";
//	outf << "\tMATERIAL MAP" << "\n";
//	m_model->save(outf);
//
//	m_camera->save(outf);
//
//	outf << "END PASS\n";
//
//}
//
//
//void CPass::load(std::ifstream &inf, std::string path) {
//
//	std::string text,item,fullName;
//	char textc[256],modelName[256],modelPath[256];
//
//
//	initVars();
//	do {
//
//		inf.getline(textc,256);
//		text = textc;
//		item = text.substr(0,text.find("=",1));
//		if (item == "\tMODEL NAME") // must read name and path before reading other attributes
//			sscanf(textc,"\tMODEL NAME=%s",modelName);
//		else if (item == "\tMODEL PATH") {
//			sscanf(textc,"\tMODEL PATH=%s",modelPath);
//			fullName = path + "/";
//			fullName += modelPath;
//
//			setModel(CFilename::GetPath(fullName),modelName);
//		}
//		else if (item == "\tRT SIZE")
//			sscanf(textc,"\tRT SIZE=%d",&m_rtSize);
//		else if (item == "\tNUM RT")
//			sscanf(textc,"\tNUM RT=%d",&m_color);
//		else if (item == "\tMATERIAL MAP")
//			m_model->load(inf);
//		else if (item == "CAMERA FLY") {
//			m_camera = new CCameraFly();
//			m_camera->load(inf);
//		}
//		else if (item == "CAMERA EXPLORE"){
//			m_camera = new CCameraExplore();
//			m_camera->load(inf);
//		}
//		else if (item == "CAMERA"){
//			m_camera = new CCamera();
//			m_camera->load(inf);
//		}
//
//	} while (text != "END PASS");
//
//
//	if (m_color  > 0)
//		m_useRT = 1;
//	setup();
//}
//
//
//void CPass::setModel(std::string path,std::string fname) {
//	
//	m_model = CEditorModel::loadModel(path, fname);
//}

	//Pass::Pass(std::string path,std::string fname) {
//	
//	//S3DSLoader loader;
//	initVars();
//	//m_model = loader.load((char *)path.c_str(),(char *)fname.c_str());
//	m_model = CEditorModel::loadModel(path, fname);
//}

	/***MARK***/ //Create Rendertargets and textures...

	//glDeleteTextures(4,m_texId);

	//GLuint attachments[5] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_DEPTH_ATTACHMENT_EXT };

	//if (m_color) {

	//	glGenFramebuffersEXT(1, &m_fbColor);
	//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbColor);

	//	glGenTextures(m_color, m_texId);
	//	for(int i=0; i < m_color; i++) {
	//		glBindTexture(GL_TEXTURE_2D, m_texId[i]);
	//	  //  	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
 //       	glTexImage2D(GL_TEXTURE_2D, 0, m_rtMode, m_rtSize, m_rtSize, 0, m_rtMode, GL_UNSIGNED_BYTE, NULL);
 //       	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 //       	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//		
 //       	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachments[i], GL_TEXTURE_2D, m_texId[i], 0);
	//		m_renderTargets[i].m_id = m_texId[i];
	//		m_renderTargets[i].m_height = m_rtSize;
	//		m_renderTargets[i].m_width = m_rtSize;
	//	}

	//	glDrawBuffers(m_color,attachments);
	//        
	//	glGenRenderbuffersEXT(1, &m_depthbuffer);
	//	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthbuffer);
	//	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_rtSize, m_rtSize);
	//	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depthbuffer);

	//	if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)!=GL_FRAMEBUFFER_COMPLETE_EXT)
	//		m_fbColor = 0;

	//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	//}
		
	//else if (m_depth) {

	//	glGenFramebuffersEXT(1, &m_fbDepth);
	//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbDepth);

	//	glGenTextures(1, &m_texId[4]);
	//	glBindTexture(GL_TEXTURE_2D, m_texId[4]);
	//	  //  	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
 //       glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_rtSize, m_rtSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
 //       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 //       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//		
 //       glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachments[4], GL_TEXTURE_2D, m_texId[4], 0);

	//	glDrawBuffers(1,&attachments[4]);
	//        
	//	if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)!=GL_FRAMEBUFFER_COMPLETE_EXT)
	//		m_fbDepth = 0;

	//	m_renderTargets[4].m_id = m_texId[4];
	//	m_renderTargets[4].m_height = m_rtSize;
	//	m_renderTargets[4].m_width = m_rtSize;

	//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	//}
//void 
//Pass::setRenderToQuad (bool value)
//{
//	m_UseScreenFillingQuad = value;
//	if (true == m_UseScreenFillingQuad) {
//		if (0 == m_QuadObject) {
//		}
//	}
//}


//void
//Pass::render (std::vector<ISceneObject*> &sceneObjects)
//{	
//	std::vector<ISceneObject*>::iterator objsIter = sceneObjects.begin();
//
//	//RENDERER->deactivateDefaultLight();
//	if (m_Lights.size())
//		RENDERER->activateLighting();
//	RENDERER->enableDepthTest();
//	RENDERER->enableTexturing();
//
//	RENDERMANAGER->clearQueue();	
//
//	for ( ; objsIter != sceneObjects.end(); objsIter++) {
//		RENDERMANAGER->addToQueue ((*objsIter), m_MaterialMap);
//	}
//
//	RENDERMANAGER->processQueue();	
//
//	RENDERER->deactivateLighting();
//	RENDERER->disableDepthTest();
//	RENDERER->disableTexturing();
//}

//void 
//Pass::_rebuild() 
//{
//
//	if (0 != m_RenderTarget) {
//		RESOURCEMANAGER->removeRenderTarget (m_RenderTarget->getName());
//		m_RenderTarget = 0;
//	}
//
//	if (m_Color > 0) {
//		m_RenderTarget = RESOURCEMANAGER->createRenderTarget (m_Name + "#rt", m_RTSizeWidth, m_RTSizeHeight);
//
//		for (int i = 0; i < m_Color; i++) {
//			std::string texName;
//			std::string internalFormat;
//			std::string format;
//			std::string type;
//
//			if (0 != m_TexId[i]) {
//				texName = m_TexId[i]->getLabel();
//				internalFormat = m_TexId[MAXFBOs]->getStringInternalFormat();
//				format = m_TexId[MAXFBOs]->getStringFormat();
//				type = m_TexId[MAXFBOs]->getStringType();
//				RESOURCEMANAGER->removeTexture (m_TexId[i]->getLabel());
//				m_TexId[i] = 0;
//			}
//
//			//std::stringstream textureName;
//
//			//textureName << m_RenderTarget->getName().c_str();
//			//textureName << "#color";
//			//textureName << i;
//
//			m_TexId[i] = RESOURCEMANAGER->createTexture 
//				(texName, internalFormat, format, type, m_RTSizeWidth, m_RTSizeHeight);
//			m_RenderTarget->bind();
//			m_RenderTarget->attachColorTexture (m_TexId[i], (RenderTarget::ColorAttachment)i);
//			m_RenderTarget->unbind();
//		}
//		m_RenderTarget->setDrawBuffers();
//	}  
//	if (m_Depth) {
//		if (0 == m_RenderTarget) {
//			m_RenderTarget = RESOURCEMANAGER->createRenderTarget (m_Name + "#rt", m_RTSizeWidth, m_RTSizeHeight);
//		}
//		std::string texName;
//		std::string internalFormat;
//		std::string format;
//		std::string type;
//
//		if (0 != m_TexId[MAXFBOs]) {
//			texName = m_TexId[MAXFBOs]->getLabel();
//			internalFormat = m_TexId[MAXFBOs]->getStringInternalFormat();
//			format = m_TexId[MAXFBOs]->getStringFormat();
//			type = m_TexId[MAXFBOs]->getStringType();
//			RESOURCEMANAGER->removeTexture (m_TexId[MAXFBOs]->getLabel());
//			m_TexId[MAXFBOs] = 0;			
//		}
//
//		//std::stringstream textureName;
//		//textureName << m_RenderTarget->getName().c_str();
//		//textureName << "#depth";
//
//		m_TexId[MAXFBOs] = RESOURCEMANAGER->createTexture 
//			(texName, internalFormat, format, type, m_RTSizeWidth, m_RTSizeHeight);
//		m_RenderTarget->bind();
//		m_RenderTarget->attachDepthTexture (m_TexId[MAXFBOs]);
//		m_RenderTarget->unbind();
//	}
//}

//void 
//Pass::addColorTarget (std::string name, std::string internalFormat, std::string format, std::string type)
//{
//	if (m_Color >= MAXFBOs) {
//		return;
//	}
//
//	if (0 == m_RenderTarget) {
//		m_RenderTarget = RESOURCEMANAGER->createRenderTarget (m_Name + "#rt", m_RTSizeWidth, m_RTSizeHeight);
//	}
//
//	m_TexId[m_Color] = RESOURCEMANAGER->createTexture 
//		(name, internalFormat,format, type, m_RTSizeWidth, m_RTSizeHeight);
//	m_RenderTarget->bind();
//	m_RenderTarget->attachColorTexture (m_TexId[m_Color], (RenderTarget::ColorAttachment)m_Color);
//	m_RenderTarget->unbind();	
//
//	m_Color++;
//	m_UseRT = true;
//
//	m_RenderTarget->setDrawBuffers();
//}
//
//void 
//Pass::addDepthTarget (std::string name, std::string internalFormat, std::string format, std::string type, bool noReadAndWrite)
//{
//	m_Depth = 1;
//
//	
//	if (0 == m_RenderTarget) {
//		m_RenderTarget = RESOURCEMANAGER->createRenderTarget (m_Name + "#rt", m_RTSizeWidth, m_RTSizeHeight);
//	}
//
//	if (0 != m_TexId[MAXFBOs]) {
//		RESOURCEMANAGER->removeTexture (m_TexId[MAXFBOs]->getLabel());
//		m_TexId[MAXFBOs] = 0;			
//	}
//
//	m_TexId[MAXFBOs] = RESOURCEMANAGER->createTexture 
//		(name, internalFormat, format, type, m_RTSizeWidth, m_RTSizeHeight);
//	m_RenderTarget->bind();
//	m_RenderTarget->attachDepthTexture (m_TexId[MAXFBOs], noReadAndWrite);
//	m_RenderTarget->unbind();
//
//	m_UseRT = true;
//}

//void
//Pass::removeLights (void)
//{
//	RENDERER->deactivateLight (*l);
//}
//
//curitiba::geometry::BoundingBox 
//Pass::getBoundingBox (std::vector<curitiba::scene::ISceneObject*> &sceneObjects)
//{
//	BoundingBox aBox;
//	std::vector<curitiba::scene::ISceneObject*>::iterator objectsIter;
//
//	objectsIter = sceneObjects.begin();
//	for (; objectsIter != sceneObjects.end(); objectsIter++) {
//		aBox.compound ((*objectsIter)->getBoundingVolume());
//	}
//	return aBox;
//}

//void 
//Pass::remapAllFromPriority(int pPriority, const std::string &materialLib, const std::string &destinyMaterialName)
//{
//	Material *m;
//	m_RemapMode = REMAP_TO_ONE;
//	//m_MaterialMap["*"].setMaterialID (materialLib, destinyMaterialName);
//
//	std::map<std::string, MaterialID>::iterator matMapIter;
//	
//	matMapIter = m_MaterialMap.begin();
//
//	for ( ; matMapIter != m_MaterialMap.end(); matMapIter++) {
//		m = MATERIALLIBMANAGER->getMaterial((*matMapIter).second);
//		if (m->getState()->getPropi(IState::PRIORITY) == pPriority)
//			(*matMapIter).second.setMaterialID (materialLib, destinyMaterialName);
//	}
//}

