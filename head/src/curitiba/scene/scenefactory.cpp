#include <curitiba/scene/scenefactory.h>
#include <curitiba/scene/octreescene.h>
#include <curitiba/scene/octreeByMatscene.h>
#include <curitiba/scene/scene.h>
#include <curitiba/scene/sceneposes.h>
#include <curitiba/scene/sceneskeleton.h>
#include <curitiba/scene/sceneaux.h>


using namespace curitiba::scene;




IScene*
SceneFactory::create(std::string scene)
{
	IScene *pScene = 0;

	if ("OctreeByMat" == scene)
		pScene = new OctreeByMatScene;

	if ("Octree" == scene) {
		pScene = new OctreeScene;
	}
	else if ("Scene" == scene) {
		pScene = new Scene;
	}
	else if ("ScenePoses" == scene) {
		pScene = new ScenePoses;
	}
	else if ("SceneSkeleton" == scene) {
		pScene = new SceneSkeleton;
	}
	else if ("SceneAux" == scene) {
		pScene = new SceneAux;
	}

	return pScene;
}
