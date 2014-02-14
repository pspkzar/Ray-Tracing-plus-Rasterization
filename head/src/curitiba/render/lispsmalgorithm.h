#ifndef LISPSMALGORITHM
#define LISPSMALGORITHM

#include <curitiba/render/irenderalgorithm.h>
#include <curitiba/scene/iscene.h>
#include <curitiba/render/irenderer.h>
#include <curitiba/render/texture.h>
#include <curitiba/render/rendertarget.h>
#include <curitiba/geometry/quad.h>

#include <curitiba/math/simpletransform.h>

namespace curitiba
{
	namespace render
	{
		class LiSPSMAlgorithm :
			public IRenderAlgorithm
		{
		public:
			LiSPSMAlgorithm(void);

			void init (void);
			void renderScene (curitiba::scene::IScene *aScene);
			void setRenderer (curitiba::render::IRenderer *aRenderer);

			void externCommand (char keyCode);
		public:
			~LiSPSMAlgorithm(void);
		
		private:
			void calculateShadow (curitiba::scene::IScene *aScene, curitiba::scene::Camera &aCamera);
			void materialPass (std::vector<curitiba::scene::ISceneObject*> &sceneObjects, curitiba::math::ITransform  &t);
			void deferredShadePass (curitiba::scene::Camera &quadCam, curitiba::scene::Camera &aCamera, curitiba::scene::Light &aLight);
			void drawShadow (curitiba::scene::Camera &quadCam, curitiba::scene::Camera &lightCam, curitiba::scene::Camera &aCamera);

			void debug (curitiba::scene::Camera &quadCam);
			void waterPass (std::vector<curitiba::scene::ISceneObject*> &sceneObjects);

			bool waterOnFrustum (curitiba::scene::IScene *aScene, std::vector<curitiba::scene::ISceneObject*> &sceneObjects, float *plane);
			void renderFixed (std::vector<curitiba::scene::ISceneObject*> &sceneObjects, curitiba::scene::Camera &aCam);

		private:
			bool m_Inited;

			curitiba::render::IRenderer *m_pRenderer;
			curitiba::render::Texture *m_ShadowTexture;
			curitiba::render::Texture *m_DepthTexture;
			curitiba::render::Texture *m_AmbientTexture;
			curitiba::render::Texture *m_NormalTexture;
			curitiba::render::Texture *m_PositionTexture;
			curitiba::render::Texture *m_WaterReflectionTexture;
			curitiba::render::Texture *m_WaterDepthTexture;
			curitiba::render::Texture *m_FinalTexture;
			curitiba::render::Texture *m_LightCamTexture;
			curitiba::render::RenderTarget *m_WaterFBO;
			curitiba::render::RenderTarget *m_RenderTarget;
			curitiba::render::RenderTarget *m_MRT;

			CProgram *m_pBlankShader;
			CProgram *m_pDeferredShader;
			CProgram *m_pDeferredShadowShader;
			CProgram *m_pMaterialPassShader;
			CProgram *m_pWaterShader;

			CTexture *m_CausticTexture;

			CMaterial *m_WaterMaterial; /***MARK***/ /*HACK!!!!*/

			curitiba::geometry::Quad *m_Quad;


			curitiba::math::SimpleTransform m_LightTransforms[4];
			int m_Split;
			bool m_RenderBoundBox;
			bool m_FixedFunc;
		};
	};
};

#endif //LISPSMALGORITHM
