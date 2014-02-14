#ifndef INTERPOLATORFACTORY_H
#define INTERPOLATORFACTORY_H

#include <curitiba/event/interpolator.h>
#include <string>

namespace curitiba
{
	namespace event_
	{
		class InterpolatorFactory
		{
		public:
			static curitiba::event_::Interpolator* create (std::string type);
		private:
			InterpolatorFactory(void) {};
			~InterpolatorFactory(void) {};
		};
	};
};
#endif