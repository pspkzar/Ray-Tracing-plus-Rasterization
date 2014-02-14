#include <curitiba/render/materialsortrenderqueue.h>

#include <curitiba.h>
#include <curitiba/render/irenderable.h>
#include <curitiba/geometry/boundingbox.h>

#include <curitiba/debug/profile.h>

using namespace curitiba::render;
using namespace curitiba::scene;
using namespace curitiba::material;
using namespace curitiba::math;
using namespace curitiba;

MaterialSortRenderQueue::MaterialSortRenderQueue(void)
{
}

MaterialSortRenderQueue::~MaterialSortRenderQueue(void)
{
}

void 
MaterialSortRenderQueue::clearQueue (void)
{
	std::map<int, std::map<Material*, std::vector<std::pair<IMaterialGroup*, ITransform*> >* >* >::iterator mapIter;

	mapIter = m_RenderQueue.begin();

	for ( ; mapIter != m_RenderQueue.end(); mapIter++) {
		std::map <Material*, std::vector<std::pair<IMaterialGroup*, ITransform*> >* > *aMap;
		std::map <Material*, std::vector<std::pair<IMaterialGroup*, ITransform*> >* >::iterator mapIter2;

		aMap = (*mapIter).second;

			mapIter2 = aMap->begin();
			for ( ; mapIter2 != aMap->end(); mapIter2++) {
				//if ((*mapIter2).second != NULL)
					delete (*mapIter2).second;
			}

			delete aMap;
	}

	m_RenderQueue.clear(); /***MARK***/ //Possible memory leak
}

void 
MaterialSortRenderQueue::addToQueue (SceneObject* aObject,
									 std::map<std::string, MaterialID> &materialMap)
{
	PROFILE ("Queue add");

	int order;
	Material* aMaterial = 0;
	MaterialLibManager *m = CURITIBA->getMaterialLibManager();

	IRenderable &aRenderable = aObject->getRenderable();

	std::vector<IMaterialGroup*> vMaterialGroups = aRenderable.getMaterialGroups();
	std::vector<IMaterialGroup*>::iterator groupIter;
	groupIter = vMaterialGroups.begin();

	for ( ; groupIter != vMaterialGroups.end(); groupIter++ ) {
		IMaterialGroup *aGroup = (*groupIter);
		IndexData &indexData = aGroup->getIndexData();
		{
			PROFILE ("Get material");
			aMaterial = materialMap[aGroup->getMaterialName()].m_MatPtr;
		}
		order = aMaterial->getState()->getPropi(IState::ORDER);
		if ((order >= 0) && (0 != aMaterial) && (true == aMaterial->isEnabled())) {

			if (0 == m_RenderQueue.count (order)){
				m_RenderQueue[order] = new std::map <Material*, std::vector<std::pair<IMaterialGroup*, ITransform*> >* >;
			}
			std::map<Material*, std::vector<std::pair<IMaterialGroup*, ITransform*> >* > *materialMap = m_RenderQueue[order];

			if (0 == materialMap->count (aMaterial)) {
				(*materialMap)[aMaterial] = new std::vector<std::pair<IMaterialGroup*, ITransform*> >;
			}
			std::vector<std::pair<IMaterialGroup*, ITransform*> > *matGroupVec = (*materialMap)[aMaterial];
			matGroupVec->push_back (std::pair<IMaterialGroup*, ITransform*>(aGroup, aObject->_getTransformPtr()));
		}
	}

// ADD BOUNDING BOXES TO QUEUE
#ifdef CURITIBA_RENDER_FLAGS
	if (CURITIBA->getRenderFlag(Curitiba::BOUNDING_BOX_RENDER_FLAG)) {

		Profile("Enqueue Bounding Boxes");

		groupIter = curitiba::geometry::BoundingBox::getGeometry()->getMaterialGroups().begin();

		for ( ; groupIter != curitiba::geometry::BoundingBox::getGeometry()->getMaterialGroups().end(); groupIter++ ) {
			IMaterialGroup *aGroup = (*groupIter);;
			Material *aMaterial = MATERIALLIBMANAGER->getMaterial(DEFAULTMATERIALLIBNAME, aGroup->getMaterialName());
			ITransform *trans = ((curitiba::geometry::BoundingBox *)(aObject->getBoundingVolume()))->getTransform();
			if (0 == m_RenderQueue.count (0)){
					m_RenderQueue[0] = new std::map <Material*, std::vector<std::pair<IMaterialGroup*, ITransform*> >* >;
				}
				std::map<Material*, std::vector<std::pair<IMaterialGroup*, ITransform*> >* > *materialMap = m_RenderQueue[aMaterial->getState()->getPropi(IState::ORDER)];

				if (0 == materialMap->count (aMaterial)) {
					(*materialMap)[aMaterial] = new std::vector<std::pair<IMaterialGroup*, ITransform*> >;
				}
				std::vector<std::pair<IMaterialGroup*, ITransform*> > *matGroupVec = (*materialMap)[aMaterial];
				curitiba::geometry::BoundingBox *bb = (curitiba::geometry::BoundingBox *)(aObject->getBoundingVolume());
				matGroupVec->push_back (std::pair<IMaterialGroup*, ITransform*>(aGroup, bb->getTransform()));
		}
	}
#endif
}


void
MaterialSortRenderQueue::processQueue (void)
{
	PROFILE ("Process queue");

	IRenderer *renderer = RENDERER;

	std::map <int, std::map<Material*, std::vector<std::pair<IMaterialGroup*, ITransform*> >* >* >::iterator renderQueueIter;

	renderQueueIter = m_RenderQueue.begin();

	for (; renderQueueIter != m_RenderQueue.end(); ++renderQueueIter) {
		std::map<Material*, std::vector<std::pair<IMaterialGroup*, ITransform*> >* >::iterator materialMapIter;

		materialMapIter = (*renderQueueIter).second->begin();

		for (; materialMapIter != (*renderQueueIter).second->end(); materialMapIter++) {
			Material *aMat = (*materialMapIter).first;
			
			{
				PROFILE ("Material prepare");
				aMat->prepare();
			}
			
			std::vector<std::pair<IMaterialGroup*, ITransform*> >::iterator matGroupsIter;

			matGroupsIter = (*materialMapIter).second->begin();
			
			{
				PROFILE ("Geometry rendering");
				for (; matGroupsIter != (*materialMapIter).second->end(); ++matGroupsIter) {
					bool b = (*matGroupsIter).second->isIdentity();
					if (!b) {
						renderer->pushMatrix();
						renderer->applyTransform (*(*matGroupsIter).second);
						aMat->setUniformValues();
					}
					{	PROFILE("Draw");
						renderer->drawGroup ((*matGroupsIter).first);
					}
					if (!b)
						renderer->popMatrix();
				}
			}
			aMat->restore();
		}
	}
}

