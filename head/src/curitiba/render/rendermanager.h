#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H


#include <vector>
#include <string>
#include <map>

#include <curitiba/render/pipeline.h>
#include <curitiba/render/irenderer.h>
#include <curitiba/render/irenderqueue.h>
//#include <curitiba/render/console.h>
#include <curitiba/scene/camera.h>
#include <curitiba/scene/iscene.h>
#include <curitiba/scene/light.h>
#include <curitiba/scene/sceneobject.h>

namespace curitiba 
{
	namespace render 
	{
/**
* Responsible for the managment of pipelines, cameras, lights
* scenes, and scene objects. It also manages the renderqueue and the renderer
*/
		class RenderManager
		{
		private:
			IRenderer* m_pRenderer;
			IRenderQueue* m_pRenderQueue;
			std::map<std::string, Pipeline*> m_Pipelines;
			std::map<std::string, curitiba::scene::Camera*> m_Cameras;
			std::map<std::string, curitiba::scene::Light*> m_Lights;
			std::map<std::string, curitiba::scene::IScene*> m_Scenes;
			std::vector<curitiba::scene::SceneObject*> m_SceneObjects;
			Pipeline *m_ActivePipeline;

		public:

			//! Required to get the engine started
			bool init();
			//! Cleans up memory
			void clear();

			//! Returns the actual renderer
			IRenderer* getRenderer (void);
			//! Renders the active pipeline
			void renderActivePipeline ();

			// OCTREE STUFF
			//! Creates an octree for every OctreeScene
			void buildOctrees();

			//! Create VBOs for every IScene, erases all vertex data, except vertex coordinates 
			void compile();
			
			// PIPELINES
			//! Checks if a given named pipeline exists
			bool hasPipeline (const std::string &pipelineName);
			//! Returns a pointer to the named pipeline
			Pipeline* getPipeline (const std::string &pipelineName);
			//! Sets the named pipeline as the active pipeline for rendering purposes
			void setActivePipeline (const std::string &pipelineName);
			//! Returns the number of pipelines
			unsigned int getNumPipelines();
			//! Returns a vector with the name of all the defined pipelines
			std::vector<std::string> *getPipelineNames();

			// PASSES
			//! Checks if a given named pass exists in the named pipeline
			bool hasPass(const std::string &pipeline, const std::string &pass);
			//! Returns the named pass. Returns NULL if the pass does not exist
			Pass *getPass(const std::string &pipeline, const std::string &pass);
			
			float *getCurrentPassParamf(std::string paramName);
			int getCurrentPassParamType(std::string paramName);
			float *getPassParamf(std::string passName, std::string paramName);
			int getPassParamType(std::string passName, std::string paramName);

			//! Returns the name of the last pass' camera from the active pipeline
			const std::string &getDefaultCameraName();

			//void reload (void);
			//void sendKeyToEngine (char keyCode); 

			//! Currently does nothing. Returns -1
			int pick (int x, int y, std::vector<curitiba::scene::SceneObject*> &objects, curitiba::scene::Camera &aCamera);
		
			////! NOP
			//void enableStereo (void);
			////! NOP
			//void disableStereo (void);

			// TRIANGLE IDS
			void prepareTriangleIDs(bool ids);
			void getVertexData(unsigned int sceneObjID, 
									 unsigned int triID);
			SceneObject *getSceneObject(int id);
			void addSceneObject(SceneObject *s);
			void deleteSceneObject(int id);

			// RENDER QUEUE
			//! Clear Renderqueue
			void clearQueue (void);
			//! Add an ISceneObject to the IRenderQueue
			void addToQueue (curitiba::scene::SceneObject *aObject, 
				std::map<std::string, curitiba::material::MaterialID> &materialMap);
			//! Calls the IRenderQueue processQueue method
			void processQueue (void);
			

			// CAMERAS
			//! Checks if the given named camera exists
			bool hasCamera (const std::string &cameraName);
			//! Returns a pointer to the given named camera
			curitiba::scene::Camera* getCamera (const std::string &cameraName);
			//! Returns the number of cameras
			unsigned int getNumCameras();
			//! Returns a vector with the name of all cameras
			std::vector<std::string> *getCameraNames();
			//! Returns the camera of the pass currently in execution. if no pass is being rendered it returns the pipeline's default camera
			curitiba::scene::Camera* getCurrentCamera();

			// VIEWPORTS
			//! Calls the Renderer to set the viewport
			void setViewport(curitiba::render::Viewport *vp);

			// LIGHTS
			//! Checks to see if the given named light exists
			bool hasLight (const std::string &lightName);
			//! Returns a pointer to the given named light. If the light does not exist it creates one
			curitiba::scene::Light* getLight (const std::string &lightName);
			//! Returns the named light. If it does not exist, it creates a light of the given class
			curitiba::scene::Light* getLight (const std::string &lightName, const std::string &lightClass);
			//! Returns the number of lights
			unsigned int getNumLights();
			//! Returns a vector with the name of all the lights
			std::vector<std::string> *getLightNames();

			// SCENES
			bool hasScene (const std::string &sceneName);
			curitiba::scene::IScene* createScene (const std::string &sceneName, const std::string &sceneType = "OctreeByMat");
			//! Return the named scene. If it does not exist it creates one
			curitiba::scene::IScene* getScene (const std::string &sceneName);
			//! Returns all the scene names, but the SceneAux type
			std::vector<std::string> *getSceneNames();
			//! Returns ALL the scene names
			std::vector<std::string> *getAllSceneNames();

			// MATERIALS
			//! Returns all the material names from the loaded scenes
			void materialNamesFromLoadedScenes (std::vector<std::string> &materials);

			//void addAlgorithm (std::string algorithm); /***MARK***/ //This shouldn't create the algorithm
			// But then again, this will, probably, not be like this
			
			//! Sets the RenderMode: Wireframe, point, solid or material
			void setRenderMode (curitiba::render::IRenderer::TRenderMode mode);

		public:
			//! Constructor
			RenderManager(void);

		public:
			//! Destructor. Calls clear to delete the data
			~RenderManager(void);
		};
	};
};
#endif // RENDERMANAGER_H
