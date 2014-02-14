#ifndef DEPTHMAPPASS_H
#define DEPTHMAPPASS_H

#include <curitiba/render/pass.h>

namespace curitiba
{
	namespace render
	{
		class DepthMapPass :
			public Pass
		{
		protected:
			//virtual void setupCamera (void);
			curitiba::scene::Camera *m_LightCamera;

		public:
			DepthMapPass(const std::string &name);
			~DepthMapPass(void);

			virtual void prepare (void);
			virtual void doPass (void);
			virtual void restore (void);
			virtual void addLight (const std::string &light);
		};
	};
};
#endif //DEPTHMAPPASS_H
