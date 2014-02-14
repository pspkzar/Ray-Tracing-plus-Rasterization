#include <curitiba/math/transformfactory.h>

#include <curitiba/math/simpletransform.h>

using namespace curitiba::math;

ITransform* 
TransformFactory::create (std::string type)
{
	if (0 == type.compare ("SimpleTransform")) {
		return new SimpleTransform;
	}
	return 0;
}
