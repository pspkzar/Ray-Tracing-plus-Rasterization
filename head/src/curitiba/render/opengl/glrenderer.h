#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

#include <curitiba/render/irenderer.h>
#include <curitiba/math/simpletransform.h>
#include <curitiba/math/mat4.h>
#include <curitiba/math/mat3.h>

#include <curitiba/geometry/frustum.h>
#include <curitiba/scene/camera.h>
#include <curitiba/render/iprogram.h>
#include <curitiba/material/material.h>
#include <curitiba/material/colormaterial.h>
#include <curitiba/math/itransform.h>
#include <curitiba/config.h>


#define LOGGING_ON
#include <curitiba/clogger.h>
//#undef LOGGING_ON

#include <curitiba/render/opengl/glstate.h>

using namespace curitiba::scene;

namespace curitiba
{
	namespace render
	{

		class GLRenderer : public curitiba::render::IRenderer
		{
		public:
			GLRenderer();
			~GLRenderer(void);

			void setProp(IRenderer::BoolProps prop, bool value);
			bool getPropb(IRenderer::BoolProps prop);

			//static void getPropTypeAndId(std::string &s, MatrixType *dt , int *id);

			//! \name Methods
			//@{
			bool init();
			virtual void setCore(bool flag);


			// RENDER
			void setRenderMode (TRenderMode mode);
			void drawGroup (curitiba::material::IMaterialGroup* aMatGroup);
			void clear (unsigned int b);

			void setDepthClearValue(float v);
			void setDepthFunc(IState::Func f);
			//void setDepthMask(bool b);
			//void enableDepthTest (void);
			//void disableDepthTest (void);
			//void enableDepthClamping (void);
			//void disableDepthClamping (void);

			void setStencilClearValue(int v);
			void setStencilMaskValue(int i);
			void setStencilFunc(IState::Func f, int ref, unsigned int mask);
			void setStencilOp(StencilOp sfail, StencilOp dfail, StencilOp dpass);

			// PRIMITIVE COUNTER
			void resetCounters (void);
			unsigned int getCounter (unsigned int c);

			// RENDER ATTRIBS
			void saveAttrib(Attribute aAttrib);
			void restoreAttrib();
			virtual void setCullFace (Face aFace);
			//virtual void setFixedFunction (bool fixed);
			void colorMask (bool r, bool g, bool b, bool a);

			// SHADERS
			void setShader (IProgram *aShader); 
			int getAttribLocation(std::string name);

			// CAMERA
			void setCamera (curitiba::scene::Camera *aCamera);
			Camera *getCamera();
			curitiba::geometry::Frustum& getFrustum (void);
			void setViewport(int width, int height);
			void setViewport(curitiba::render::Viewport *vp);	

			// MATRICES
			void setMatrixMode (MatrixMode mode);
			void loadIdentity (void);
			const float *getMatrix(MatrixType mode);

			void translate (curitiba::math::vec3 &aVec);
			void scale (curitiba::math::vec3 &aVec);
			void rotate(float angle, curitiba::math::vec3 &axis);
			void applyTransform (const curitiba::math::ITransform &aTransform);

			void pushMatrix (void);
			void popMatrix (void);

			float* getProjectionModelviewMatrix (void);


			// TEXTURING
			void enableTexturing (void);
			void disableTexturing (void);
			void setActiveTextureUnit (TextureUnit aTexUnit);
			void enableTextureCoordsGen (void);
			void disableTextureCoordsGen (void);
			void addTexture(TextureUnit aTexUnit, Texture *t);
			void removeTexture(TextureUnit aTexUnit);
			int getPropi(TextureUnit aTexUnit, Texture::IntProperty prop);
			int getProps(TextureUnit aTexUnit, Texture::SamplerProperty prop);
			int getTextureCount();

			// STATE
			void setState (IState *aState);
			void setDefaultState();

			// LIGHTING
			virtual bool addLight (curitiba::scene::Light& aLight);
			virtual void removeLights ();
			virtual int getLightCount();
			virtual float* getLightfvComponent(int id,Light::Float4Property component);
			virtual float getLightfComponent(int id, Light::FloatProperty component);
			virtual int getLightbComponent(int id, Light::BoolProperty component);
			virtual int getLighteComponent(int id, Light::EnumProperty component);
			virtual std::string getLightName(int id);

			// COLOR AND MATERIALS
			virtual void setMaterial(const curitiba::material::ColorMaterial &mat);
			virtual void setMaterial(float *diffuse, float *ambient, float *emission, float *specular, float shininess);
			virtual const float * getColor(ColorMaterial::ColorComponent aColor);
			virtual void setColor (float r, float g, float b, float a);
			virtual void setColor (int r, int g, int b, int a);


			//CLIP PLANES
			void activateUserClipPlane (ClipPlane aClipPlane);
			void setUserClipPlane (ClipPlane aClipPlane, double *plane);
			void deactivateUserClipPlane (ClipPlane aClipPlane);

			// STEREO
			//void enableStereo (void);
			//void disableStereo (void);
			//bool isStereo (void);

			// MISC
			//void renderBoundingVolume (const curitiba::geometry::IBoundingVolume* aBoundingVolume);
			curitiba::math::vec3 readpixel (int x, int y);

			void flush (void);

#ifdef COMPATIBILITY_FLAG_OPENGL
			// FOG
			virtual void enableFog (void); 
			virtual void disableFog (void);

			//LIGHTING
			virtual void activateLighting (void);
			virtual void deactivateLighting (void);
			virtual void positionLight (curitiba::scene::Light& aLight);
#endif

		private:
			unsigned int translate(IState::Func aFunc);
			unsigned int translate(StencilOp aFunc);

			std::vector<Light *> m_Lights;
			Camera *m_Camera;

			unsigned int m_LightsOn;

			std::vector<Texture *> m_Textures;

			std::vector<SimpleTransform> m_Matrices;
			IRenderer::MatrixMode m_MatrixMode;
			ITransform *m_CurrentMatrix;

			// pre alocated memory to return composed matrices
			mat4 m_pReturnMatrix;
			mat3 m_pReturnMat3;

			std::vector<SimpleTransform> m_MatrixStack[IRenderer::COUNT_MATRIXMODE];

			GLuint m_AtomicCountersBuffer;
			int m_TriCounter;
			unsigned int *userCounters;
			void accumTriCounter(unsigned int drawPrimitive, unsigned int size);

			curitiba::render::GlState m_glCurrState, m_glDefaultState; 
			float m_ReturnMatrix[16];

			curitiba::material::ColorMaterial m_Material;

			int m_TexturingFlag;
			IRenderer::TRenderMode m_PrevRenderMode, m_ActualRenderMode;

			IProgram *m_Shader; 


			GLenum translateFace (Face aFace);
			GLenum translateMaterialComponent (ColorMaterial::ColorComponent aMaterialComponent);
			unsigned int translateDrawingPrimitive(unsigned int aDrawPrimitive);

		//	bool m_FixedFunction;
			//bool m_Stereo;
			//void doRender (curitiba::scene::ISceneObject *aRenderable, unsigned int buffers, int priority);
			//std::multimap<int, curitiba::render::IRenderable*> m_RenderQueue;
			//void setDepthCompare();
			//std::map<int, std::map<Material*, std::vector<std::pair<curitiba::materials::IMaterialGroup*, curitiba::math::ITransform*>>*>* > m_RenderQueue;
			//void drawElements (unsigned int size, std::vector<unsigned int>& indices, unsigned int aDrawingPrimitive = TRIANGLES);
			//virtual void setMaterialProvider (curitiba::scene::IScene *aScene); /***MARK***/ //!!!!!!!
			//virtual void activateDefaultLight (void);
			//virtual void deactivateDefaultLight (void);
		};
	};
};

#endif
