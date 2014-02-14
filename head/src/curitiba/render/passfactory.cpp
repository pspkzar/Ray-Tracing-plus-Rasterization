#include <curitiba/render/passfactory.h>

#include <vector>

#include <curitiba/render/pass.h>

#include <curitiba/render/depthmappass.h>
//#include <curitiba/render/depthmap4depthtexturespass.h>
//#include <curitiba/render/depthmaprgba32fpass.h>
//#include <curitiba/render/fogwithcausticspass.h>
#include <curitiba/render/quadpass.h>
//#include <curitiba/render/waterplanefogpass.h>
#include <curitiba/render/profilerpass.h>

#ifdef CURITIBA_OPTIX
#include <curitiba/render/passOptix.h>
#endif
// DAVE
//#include <curitiba/render/raytracerpass.h>
//#include <curitiba/render/shadowmapraytracerpass.h>
//END DAVE
using namespace curitiba::render;

Pass*
PassFactory::create (const std::string &type, const std::string &name)
{
	if ("default" == type) {
		return new Pass (name);
	}
	if ("depthmap" == type) {
		return new DepthMapPass (name);
	}
	if ("quad" ==  type) {
		return new QuadPass (name);
	}
	if ("profiler" == type) {
		return new ProfilerPass(name);
	}
#ifdef CURITIBA_OPTIX
	if ("optix" == type)
		return new PassOptix(name);
#endif
// DAVE
	//if ("raytracer" ==  type) {
	//	return new RayTracerPass (name);
	//}
	//////////////////////NOVO///////////
	//if ("shadowmapraytracer" ==  type) {
	//	return new ShadowMapRayTracerPass (name);
	//}
// END DAVE
	//if ("fogwithcaustics" == type) {
	//	return new FogWithCausticsPass (name);
	//}
//	if ("waterplanefog" == type) {
//		return new WaterPlaneFogPass (name);
//	}
	//if ("depthmap4depthtextures" == type) {
	//	return new DepthMap4DepthTexturesPass (name);
	//}
	//if ("depthmaprgba32f" == type) {
	//	return new DepthMapRGBA32FPass (name);
	//}

	return 0;
}

bool
PassFactory::isClass(const std::string &name)
{
	if (("default" != name) && 
		("depthmap" != name) && ("quad" != name) && ("profiler" != name) 
#ifdef CURITIBA_OPTIX		
		&& ("optix" != name)
#endif
// DAVE		
		 //&& ("raytracer" != name) && ("shadowmapraytracer" != name)
// END DAVE
		)
		return false;
	else
		return true;
}


std::vector<std::string> * 
PassFactory::getClassNames(){

	std::vector<std::string> *names = new std::vector<std::string>; 

	names->push_back("default");
	names->push_back("depthmap");
	names->push_back("quad");
	names->push_back("profiler");
	names->push_back("optix");
// DAVE
	//names->push_back("raytracer");
	//names->push_back("shadowmapraytracer");
// END DAVE
	return names;
}
