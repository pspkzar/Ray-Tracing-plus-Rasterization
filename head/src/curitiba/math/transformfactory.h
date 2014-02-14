#ifndef TRANSFORMFACTORY_H
#define TRANSFORMFACTORY_H

#include <string>

#include <curitiba/math/itransform.h>

namespace curitiba
{
	namespace math
	{
		class TransformFactory
		{
		public:
			static ITransform* create (std::string type);
		private:
			TransformFactory(void);
			~TransformFactory(void);
		};
	};
};

#endif
