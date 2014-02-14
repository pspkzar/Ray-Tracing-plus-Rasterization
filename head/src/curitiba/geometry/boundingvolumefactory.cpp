#include <curitiba/geometry/boundingvolumefactory.h>

#include <curitiba/geometry/boundingbox.h>

using namespace curitiba::geometry;

IBoundingVolume* 
BoundingVolumeFactory::create (std::string type)
{
	if (0 == type.compare ("BoundingBox")) {
		return new BoundingBox();
	}
	return 0;
}

