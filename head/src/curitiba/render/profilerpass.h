#ifndef PROFILERPASS_H
#define PROFILERPASS_H

#include <curitiba/render/pass.h>
#include <curitiba/resource/font.h>
#include <curitiba/scene/camera.h>
#include <curitiba/render/irenderable.h>

namespace curitiba
{
	namespace render
	{
		class ProfilerPass :
			public Pass
		{
		private:
			curitiba::resource::Font m_pFont;
			curitiba::scene::Camera *m_pCam;
			curitiba::scene::SceneObject *m_pSO;
		public:
			ProfilerPass (const std::string &name);

			void prepare (void);
			void restore (void);
			void doPass (void);
			virtual void setCamera (const std::string &cameraName);
			
			virtual ~ProfilerPass(void);

		};
	};
};
#endif 
