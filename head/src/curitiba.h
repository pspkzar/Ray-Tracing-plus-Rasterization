#ifndef CURITIBA_H
#define CURITIBA_H

#pragma warning( disable: 4290)

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

//#include <GL/glew.h>
//#include <IL/il.h>
#include <iostream>

#include <curitiba/errors.h>
#include <curitiba/config.h>

//#include <curitiba/animation/ianimation.h>
//#include <curitiba/animation/linearanimation.h> /***MARK***/ //A factory perhaps?
#include <curitiba/world/iworld.h>
#include <curitiba/math/vec3.h>
#include <curitiba/math/vec4.h>
#include <curitiba/material/materiallibmanager.h>
#include <curitiba/render/pipeline.h>
#include <curitiba/render/pass.h>
#include <curitiba/render/viewport.h>
#include <curitiba/render/rendermanager.h>
#include <curitiba/resource/resourcemanager.h>
#include <curitiba/scene/iscene.h>
#include <curitiba/scene/camera.h>
#include <curitiba/scene/light.h>

#include <curitiba/event/eventManager.h>
#include <curitiba/event/ilistener.h>


using namespace curitiba;

//I know Peter, but you'll see that this may come in handy ;)
#define RENDERER Curitiba::getInstance()->getRenderManager()->getRenderer()
#define RENDERMANAGER Curitiba::getInstance()->getRenderManager()
#define MATERIALLIBMANAGER Curitiba::getInstance()->getMaterialLibManager()
#define RESOURCEMANAGER Curitiba::getInstance()->getResourceManager()
#define CURITIBA Curitiba::getInstance()

#define EVENTMANAGER Curitiba::getInstance()->getEventManager()

namespace curitiba {
	
	const double NO_TIME = -1.0f;

	class Curitiba : public IListener
	{

	public:

		typedef enum DataType {
						INT,
						SAMPLER,
						BOOL,
						IVEC2,
						IVEC3,
						IVEC4,
						BVEC2,
						BVEC3,
						BVEC4,
						FLOAT,
						VEC2,
						VEC3,
						VEC4,
						MAT3,
						MAT4				
		};
		

		typedef enum RenderFlags {
						BOUNDING_BOX_RENDER_FLAG, 
						PROFILE_RENDER_FLAG, 
						COUNT_RENDER_FLAGS
		};

		static curitiba::Curitiba* create (void);
		static curitiba::Curitiba* getInstance (void);
		bool init(bool context, std::string aConfigFile = "");
		std::string &getName();

		void eventReceived(const std::string &sender, const std::string &eventType, IEventData *evt);

		void setActiveCameraName(const std::string &aCamName);
		curitiba::scene::Camera *getActiveCamera();

		//void setProfileMaterial(std::string aMaterial);

		curitiba::world::IWorld& getWorld (void);

		void step (void);

		void loadAsset (std::string aFilename, std::string sceneName, std::string params = "") throw (std::string);
		void writeAssets (std::string fileType, std::string aFilename, std::string sceneName);

		void setWindowSize (float width, float height);
		float getWindowHeight();
		float getWindowWidth();
		curitiba::render::Viewport* createViewport (const std::string &name, const curitiba::math::vec4 &bgColor);
		curitiba::render::Viewport* createViewport (const std::string &name);
		curitiba::render::Viewport* getViewport (const std::string &name);
		curitiba::render::Viewport* getDefaultViewport ();
		std::vector<std::string> *getViewportNames();

		bool reload (void);

		void sendKeyToEngine (char keyCode); /***Change this in to a register system. The sub-system register as a particular key receiver*/

		void enablePhysics (void);
		void disablePhysics (void); 

		int picking (int x, int y, std::vector<curitiba::scene::SceneObject*> &objects, curitiba::scene::Camera &aCamera);

		//void addAnimation (std::string animationName, curitiba::animation::IAnimation *anAnimation);
		//curitiba::animation::IAnimation *getAnimation (std::string animationName);

		//void enableStereo (void);
		//void disableStereo (void);
		
		/* Readers */
		void readModel (std::string fileName) throw (std::string);
		void appendModel(std::string fileName);
		void readProjectFile (std::string file, int *width, int *height);
		void Curitiba::readDirectory (std::string dirName);

		/* Managers */
		curitiba::render::RenderManager* getRenderManager (void);
		curitiba::resource::ResourceManager* getResourceManager (void);
		curitiba::material::MaterialLibManager* getMaterialLibManager (void);
		curitiba::event_::EventManager* getEventManager (void);

		/* Render Flags */
		void setRenderFlag(RenderFlags aFlag, bool aState);
		bool getRenderFlag(RenderFlags aFlag);

		std::string getDataType(DataType dt);
		DataType getDataType(std::string &s);

		~Curitiba (void);
		void clear();
	
	private:
		Curitiba();

		std::string m_Name;

		/*
		 * Rendering Flags
		 */
		std::vector<bool> m_RenderFlags;
		//curitiba::material::Material *m_ProfileMaterial; 
		bool m_UseTangents, m_UseTriangleIDs, m_CoreProfile;
		/*
		 * Managers
		 */
		curitiba::render::RenderManager *m_pRenderManager;
		curitiba::resource::ResourceManager *m_pResourceManager;
		curitiba::material::MaterialLibManager *m_pMaterialLibManager;
		curitiba::event_::EventManager *m_pEventManager;

		/*
		 * Members
		 */
		std::string m_ActiveCameraName;
		float m_WindowWidth, m_WindowHeight;
		curitiba::world::IWorld *m_pWorld;
		std::map <std::string, curitiba::render::Viewport*> m_vViewports;
		curitiba::render::Viewport *m_Viewport;
		//std::map <std::string, curitiba::animation::IAnimation*> m_Animations;
		IState *m_DefaultState;

		bool m_Inited;
		bool m_Physics;
		
		double m_CurrentTime;
		double m_LastFrameTime;

		double CLOCKS_PER_MILISEC;
		double INV_CLOCKS_PER_MILISEC;

		// different perspective and camera position depending on whether
		// the model is unitized
		void Curitiba::loadFilesAndFoldersAux(char *sceneName, bool unitize);

		int loadedScenes;

//		curitiba::scene::IScene* m_pScene;
//		curitiba::render::Console *m_pConsole;
		//curitiba::scene::IScene& getScene (void);
		//void Curitiba::resetScene (void);

	};
};

#endif //CURITIBA_H
