#ifndef GLRENDERTARGET_H
#define GLRENDERTARGET_H

#include <assert.h>

#include <curitiba/render/rendertarget.h>

#include <GL/glew.h>

#define CHECK_FRAMEBUFFER_STATUS() \
{ \
	GLenum status; \
	status = glCheckFramebufferStatus (GL_FRAMEBUFFER); \
	switch (status) { \
		case GL_FRAMEBUFFER_COMPLETE: \
			break; \
		case GL_FRAMEBUFFER_UNSUPPORTED: \
			break; \
		default: \
			assert (0); \
	} \
} \

namespace curitiba
{
	namespace render
	{

		class GLRenderTarget : public RenderTarget
		{
		friend class RenderTarget;

		private:
			//unsigned int m_Depth; 
			//unsigned int m_Color; // number of color render targets
			GLuint m_DepthBuffer;
			GLenum m_RenderTargets[RenderTarget::RENDERTARGETS];
			int m_RTCount;
			bool m_NoDrawAndRead;

		public:
			virtual void bind (void);			
			virtual void unbind (void);

			virtual bool checkStatus();

			void addColorTarget (std::string name, std::string internalFormat, std::string format="", std::string type="");
			void addDepthTarget (std::string name, std::string internalFormat, std::string format, 
				std::string type, bool noReadAndWrite = false);
			void addStencilTarget (std::string name);
			void addDepthStencilTarget(std::string name);
			~GLRenderTarget(void);

		private:
			virtual bool attachDepthStencilTexture (Texture* aTexture, GLuint type);
			virtual bool dettachDepthStencilTexture (GLuint type);
			virtual bool attachColorTexture (curitiba::render::Texture* aTexture, ColorAttachment colorAttachment); 
			virtual bool dettachColorTexture (ColorAttachment colorAttachment);

			void setDrawBuffers (void);

			GLRenderTarget (std::string name, unsigned int width, unsigned int height);
			GLRenderTarget (std::string name);

			void _rebuild();
		};
	};
};


#endif
