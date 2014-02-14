#include <curitiba/geometry/primitive.h>

#include <curitiba/geometry/box.h>
#include <curitiba/geometry/axis.h>
#include <curitiba/geometry/bbox.h>
#include <curitiba/geometry/sphere.h>


using namespace curitiba::geometry;

const std::string Primitive::NoParam = "";

//Primitive *
//Primitive::Create(const std::string &type) 
//{
//	if ("Box" == type)
//		return new Box();
//	else if ("Axis" == type)
//		return new Axis();
//	else if ("BoundingBox" == type)
//		return new BBox();
//	else if ("Sphere" == type)
//		return new Sphere();
//	else
//		return new Box();
//}


const std::string &
Primitive::getParamfName(unsigned int i) 
{
	return Primitive::NoParam;
}