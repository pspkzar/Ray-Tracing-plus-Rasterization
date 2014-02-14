#include <curitiba/scene/sceneaux.h>
#include <curitiba/render/rendermanager.h>


#include <curitiba/debug/profile.h>

using namespace curitiba::scene;
using namespace curitiba::geometry;
using namespace curitiba::render;
using namespace curitiba::material;

SceneAux::SceneAux(void) : Scene()
{
}

SceneAux::~SceneAux(void)
{
}


void
SceneAux::compile()
{
}


std::string 
SceneAux::getType (void) {
	return "SceneAux";
}

void SceneAux::unitize() {


}

