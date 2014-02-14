#ifndef QUADPASS_H
#define QUADPASS_H

#include <curitiba/render/pass.h>
#include <curitiba/geometry/quad.h>

namespace curitiba
{
	namespace render
	{
		class QuadPass :
			public Pass
		{
		private:
			curitiba::geometry::Quad *m_QuadObject;
		public:
			QuadPass (const std::string &name);

			void prepare (void);
			void restore (void);
			void doPass (void);
			
			~QuadPass(void);

		};
	};
};
#endif 
