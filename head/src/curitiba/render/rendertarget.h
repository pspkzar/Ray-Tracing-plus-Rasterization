#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <curitiba/render/rendertarget.h>

#include <curitiba/render/texture.h>

namespace curitiba
{
	namespace render
	{

		class RenderTarget
		{
		public:
			static const int RENDERTARGETS = 16;
			static const int MAXFBOs = 8;

			enum ColorAttachment {
				COLOR0,
				COLOR1,
				COLOR2,
				COLOR3,
				COLOR4,
				COLOR5,
				COLOR6,
				COLOR7,
				COLOR8,
				COLOR9,
				COLOR10,
				COLOR11,
				COLOR12,
				COLOR13,
				COLOR14,
				COLOR15
			};

		protected:
			unsigned int m_Id; 
			unsigned int m_Color; // number of color targets;
			unsigned int m_Depth;
			unsigned int m_Stencil;
			unsigned int m_Samples;
			std::string m_Name;
			curitiba::render::Texture* m_TexId[MAXFBOs+2];

			// clear values per channel
			curitiba::math::vec4 m_ClearValues;

			unsigned int m_Width;
			unsigned int m_Height;

		public:
			static RenderTarget* create (std::string name, unsigned int width, unsigned int height);
			static RenderTarget* create (std::string name);

			virtual bool checkStatus() = 0;

			virtual void bind (void) = 0;
			virtual void unbind (void) = 0;
			void setClearValues(float r, float g, float b, float a);
			void setSampleCount(int samples);
			const curitiba::math::vec4 & getClearValues(); 
			virtual void addColorTarget (std::string name, std::string internalFormat, std::string format, std::string type) = 0;
			virtual void addDepthTarget (std::string name, std::string internalFormat, std::string format, 
				std::string type, bool noReadAndWrite = false) = 0;
			virtual void addStencilTarget (std::string name) = 0;
			virtual void addDepthStencilTarget(std::string name) = 0;

			virtual unsigned int getNumberOfColorTargets();
			curitiba::render::Texture* getTexture(unsigned int i);
			virtual int getId (void);
			virtual std::string &getName (void);

			unsigned int getWidth (void);
			unsigned int getHeight (void);

			virtual ~RenderTarget(void) {};

		protected:
			RenderTarget () {};
			RenderTarget (std::string name, unsigned int width, unsigned int height);

		};
	};
};

#endif 
