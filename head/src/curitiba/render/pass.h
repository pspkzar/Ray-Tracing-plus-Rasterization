#ifndef PASS_H
#define PASS_H

#include <vector>
#include <string>
#include <map>

#include <curitiba/geometry/boundingbox.h>
#include <curitiba/geometry/quad.h>
#include <curitiba/material/materialid.h>
#include <curitiba/scene/camera.h>
#include <curitiba/scene/iscene.h>
#include <curitiba/scene/sceneobject.h>
#include <curitiba/render/rendertarget.h>
#include <curitiba/render/texture.h>
#include <curitiba/scene/geometryobject.h>

#include <curitiba/event/eventManager.h>
#include <curitiba/event/ilistener.h>
#include <curitiba/event/eventString.h>

namespace curitiba
{
	namespace render
	{

		class Pass : public IListener {

		public:

			enum TYPES{
				NOT_DEFINED = -1,
				FLOAT,
				STRING
			};
			//	FLOAT_ARRAY,
			//	INT
			//};

			static const int MAXFBOs = 4;

			void setProp(IRenderer::BoolProps prop, bool value);
			bool getPropb(IRenderer::BoolProps prop);

		private:
			std::string p_Empty;
			void initVars();

		protected:

			std::vector<bool> m_BoolProp;

			std::string m_ClassName;
			std::string m_Name;
			std::string m_CameraName;
			std::vector<std::string> m_SceneVector;
			std::map<std::string, curitiba::material::MaterialID> m_MaterialMap;
			curitiba::render::Viewport *m_pViewport;
			curitiba::render::Viewport *m_pRestoreViewport;
						
			curitiba::render::RenderTarget *m_RenderTarget;
			curitiba::render::Texture* m_TexId[MAXFBOs+1];	
			curitiba::render::Texture* m_Inputs[8];
			//curitiba::render::RenderTarget* m_DepthBuffer;

			std::map<std::string, float> m_Paramf;
			std::map<std::string, std::string> m_Params;
			std::map<std::string, TYPES> m_ParamType;


			//bool m_DoColorClear;
			vec4 m_ColorClearValue;

			//bool m_DoDepthClear;
			float m_DepthClearValue;
			IState::Func m_DepthFunc;
			//bool m_DepthMask;

			//bool m_DoStencilClear;
			int m_StencilClearValue;
			int m_StencilMaskValue;
			IRenderer::StencilOp m_Stencilsfail, m_Stencildfail, m_Stencildpass;
			IState::Func m_StencilFunc;
			int m_StencilOpRef;
			unsigned int m_StencilOpMask;

			int m_RTSizeWidth; // size of render targets
			int m_RTSizeHeight;

			int m_Depth; 
			int m_Color; // number of render targets

			bool m_UseRT;

			typedef enum {REMAP_DISABLED, REMAP_TO_ONE, REMAP_TO_LIBRARY} RemapMode;
			
			RemapMode m_RemapMode;

			std::vector<std::string> m_Lights;

			//std::vector<std::string> m_ParamNames;
			//std::vector<TYPES> m_ParamTypes;

		public:
			Pass (const std::string &passName);
			//Pass(std::string path,std::string fname);
			virtual ~Pass();

			void eventReceived(const std::string &sender, const std::string &eventType, IEventData *evtData);

			const std::string &getClassName();

			virtual const std::map<std::string, float> &getParamsf();
			virtual void setParam(const std::string &name, const float value);
			virtual void setParam(const std::string &name, const std::string &value);			//virtual void setParam(std::string name, int value) {};
			//virtual void setParamv(std::string name, int count, float *values) {};
		
			virtual float *getParamf(const std::string &name);
			virtual int getParamType(const std::string &name);
			virtual std::string &getParams(const std::string &name);			
			//virtual int getParami(std::string name) {return (0);};
			//virtual float *getParamfv(std::string name) {return (NULL);};
			//virtual std::vector<std::string> &getParamNames() {return(m_ParamNames);};
			//virtual std::vector<TYPES> &getParamTypes(){return m_ParamTypes; };

			std::string &getName (void);



			virtual void addScene (const std::string &sceneName);
			bool hasScene(const std::string &name);
			void removeScene(const std::string &name);

			void updateMaterialMaps(const std::string &sceneName);

			const std::vector<std::string>& getScenesNames (void);

			virtual void setCamera (const std::string &cameraName);
			const std::string& getCameraName (void);

			void setViewport (curitiba::render::Viewport *aViewport);
			curitiba::render::Viewport *getViewport();
			
			//void setDoColorClear (bool value);
			void setColorClearValue(float r, float g, float b, float  a);
			//bool getDoColorClear (void);

			//void setDoDepthClear (bool value);
			void setDepthClearValue(float value);
			void setDepthFunc(IState::Func f);
			//void setDepthMask(bool b);
			//bool getDoDepthClear (void);

			//void setDoStencilClear (bool value);
			void setStencilClearValue(float value);
			void setStencilMaskValue(int i);
			void setStencilFunc(IState::Func f, int ref, unsigned int mask);
			void setStencilOp(	IRenderer::StencilOp sfail, 
							IRenderer::StencilOp dfail, 
							IRenderer::StencilOp dpass);

			//bool getDoStencilClear (void);


			virtual void prepare (void);
			virtual void restore (void);
			virtual bool renderTest (void);
			virtual void doPass (void);

			/*Lights*/
			virtual void addLight (const std::string &name);
			bool hasLight(const std::string &name);
			void removeLight(const std::string &name);
				
			/*Materials*/
			const std::map<std::string, curitiba::material::MaterialID> &getMaterialMap();
			void remapMaterial (const std::string &originMaterialName, 
								const std::string &materialLib, 
								const std::string &destinyMaterialName);
			void remapAll (const std::string &materialLib, 
								const std::string &destinyMaterialName);
			void remapAll (const std::string &targetLibrary);
			//void remapAllFromPriority(int pPriority, 
			//					const std::string &pToLibrary, 
			//					const std::string &pToMaterial);

			void materialNamesFromLoadedScenes (std::vector<std::string> &materials);

			/*Rendertargets*/
			curitiba::render::RenderTarget* getRenderTarget (void);
			virtual void setRenderTarget (curitiba::render::RenderTarget* rt);
			void enableRenderTarget(bool b);
			bool isRenderTargetEnabled();

			bool hasRenderTarget();
		
		protected:
			virtual void setupCamera (void);
			void restoreCamera (void);
			void setupLights (void);

			void prepareBuffers();

			void setRTSize (int width, int height);
		/***MARK***/ //Maybe this should be moved to the BoundingBox class
			curitiba::geometry::BoundingBox getBoundingBox (std::vector<curitiba::scene::SceneObject*> &sceneObjects);

			//void removeLights (void);
			//void setup();
			//void _rebuild();
			//void render (std::vector<curitiba::scene::ISceneObject*> &sceneObjects);
			//void setRTMode (curitiba::render::Texture::TextureFormat rtMode);

			//void addColorTarget (std::string name, std::string internalFormat, std::string format, std::string type);
			//void addDepthTarget (std::string name, std::string internalFormat, std::string format, 
			//	std::string type, bool noReadAndWrite = false);

			//void setFBOs (int numFBOs);
			//void setInputs (int input, std::string label); 
			//curitiba::render::Texture::TextureFormat m_RTMode;
			//CameraType getCameraType (void);
			//void setCameraType (CameraType aType);
			//enum CameraType {
			//	SCENE,
			//	LIGHT,
			//	CUSTOM
			//};
			//CameraType m_CameraType;

		};
	};
};
#endif





	
	
