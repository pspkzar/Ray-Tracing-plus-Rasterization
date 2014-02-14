#include <curitiba/render/profilerpass.h>
#include <curitiba/resource/fontmanager.h>
#include <curitiba/scene/sceneobject.h>
#include <curitiba/debug/profile.h>
#include <curitiba/geometry/mesh.h>




#include <curitiba.h>



#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG


using namespace curitiba::render;


ProfilerPass::ProfilerPass (const std::string &name) :
	Pass (name), m_pCam(0)
{
	m_ClassName = "profiler";

	m_CameraName = "__ProfilerCamera";
	m_pCam = RENDERMANAGER->getCamera("__ProfilerCamera");
	m_pSO = SceneObjectFactory::create("SimpleObject");
	m_pSO->setRenderable(RESOURCEMANAGER->createRenderable("Mesh", "__ProfilerResult", "Profiler"));

	m_pViewport = CURITIBA->createViewport("__Profiler", vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pViewport = new curitiba::render::Viewport();
	m_pCam->setViewport(m_pViewport);
	m_pViewport->setSize(CURITIBA->getWindowWidth(),CURITIBA->getWindowWidth());
	m_pViewport->setOrigin(0,0);
	m_pCam->setProjectionType(curitiba::scene::Camera::ORTHO);
	m_pCam->setOrtho(0.0f ,CURITIBA->getWindowWidth() , CURITIBA->getWindowWidth(), 0.0f, -1.0f, 1.0f);

	m_pFont = FontManager::getFont("CourierNew10");

	m_MaterialMap[m_pFont.getMaterialName()] = MaterialID(DEFAULTMATERIALLIBNAME,m_pFont.getMaterialName());

	m_BoolProp[IRenderer::COLOR_CLEAR] = false;
}


ProfilerPass::~ProfilerPass(void)
{
}


void
ProfilerPass::setCamera (const std::string &cameraName) {

}


void
ProfilerPass::prepare (void)
{
	if (0 != m_RenderTarget && true == m_UseRT) {
		m_RenderTarget->bind();
	}

	RENDERER->setMatrixMode (IRenderer::PROJECTION_MATRIX);
	RENDERER->pushMatrix();
	RENDERER->loadIdentity();

	RENDERER->setMatrixMode (IRenderer::VIEW_MATRIX);
	RENDERER->pushMatrix();
	RENDERER->loadIdentity();



//	vec3 v = m_pViewport->getSize();

	prepareBuffers();

	m_pViewport->setSize(CURITIBA->getWindowWidth(),CURITIBA->getWindowHeight());
	m_pCam->setOrtho(0.0f ,CURITIBA->getWindowWidth() , CURITIBA->getWindowHeight(), 0.0f, -1.0f, 1.0f);

	if (m_pViewport != NULL) {
		RENDERER->setViewport(m_pViewport);
	}
	RENDERER->setCamera(m_pCam);
	
	RENDERER->setMatrixMode(curitiba::render::IRenderer::MODEL_MATRIX);
	RENDERER->loadIdentity();
	RENDERER->pushMatrix();
	RENDERER->translate(vec3(15,15,0));

#if (CURITIBA_CORE_OPENGL == 0)
	RENDERER->activateLighting();
	RENDERER->enableTexturing();
#endif
}


void
ProfilerPass::restore (void)
{
	if (0 != m_RenderTarget && true == m_UseRT) {
		m_RenderTarget->unbind();
	}

	RENDERER->setMatrixMode (IRenderer::PROJECTION_MATRIX);
	RENDERER->popMatrix();

	RENDERER->setMatrixMode (IRenderer::VIEW_MATRIX);
	RENDERER->popMatrix();

	RENDERER->setMatrixMode (IRenderer::MODEL_MATRIX);
	RENDERER->popMatrix();

#if (CURITIBA_CORE_OPENGL == 0)
	RENDERER->disableTexturing();
#endif
}


void 
ProfilerPass::doPass (void)
{
//	curitiba::scene::SceneObject *string;

	m_pFont.createSentenceRenderable(m_pSO->getRenderable(), Profile::DumpLevels());
	m_pSO->getRenderable().resetCompilationFlags();
	//m_pSO->setRenderable(r);//Profile::DumpLevels()));
	//string = m_pFont.createSentenceSceneObject(Profile::DumpLevels());
	RENDERMANAGER->clearQueue();
	RENDERMANAGER->addToQueue (m_pSO, m_MaterialMap);
	RENDERMANAGER->processQueue();
}

