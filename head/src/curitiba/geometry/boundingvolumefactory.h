#ifndef BOUNDINGVOLUMEFACTORY_H
#define BOUNDINGVOLUMEFACTORY_H

#include <curitiba/geometry/iboundingvolume.h>

namespace curitiba
{
	namespace geometry
	{
		class BoundingVolumeFactory
		{
		public:
			static IBoundingVolume* create (std::string type);

		private:
			BoundingVolumeFactory(void);
			~BoundingVolumeFactory(void);
		};
	};
};

#endif
