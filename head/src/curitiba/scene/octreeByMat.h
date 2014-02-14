#ifndef OCTREEBYMAT_H
#define OCTREEBYMAT_H

#include <curitiba/scene/octreeByMatnode.h>
#include <curitiba/scene/iscene.h>
#include <curitiba/scene/camera.h>
#include <curitiba/math/vec3.h>
#include <curitiba/geometry/boundingbox.h>
#include <curitiba/clogger.h> /***MARK***/

#include <vector>
#include <string>

namespace curitiba {
	namespace loader {
		class CBOLoader;
	};
};

namespace curitiba
{

	namespace scene
	{

		//class COctreeNode;
		class OctreeByMat {

			friend class curitiba::loader::CBOLoader;
			friend class OctreeByMatNode;

		public:
			OctreeByMat();

			virtual ~OctreeByMat();
			void setName(std::string name);
			std::string getName();
			void build (std::vector<curitiba::scene::SceneObject*> &sceneObjects);
		
			void updateOctreeTransform(curitiba::math::ITransform *m_Transform);
			int getNumberOfVertices () { return 0; };

			curitiba::math::vec3& getVertice (unsigned int v);
			void unitize(float min, float max);
			void _compile (void);
			void _findVisibleSceneObjects (std::vector<curitiba::scene::SceneObject*> &m_vReturnVector,
																curitiba::geometry::Frustum &aFrustum, 
																curitiba::scene::Camera &aCamera,
																bool conservative = false);

			void _getAllObjects (std::vector<curitiba::scene::SceneObject*> &m_vReturnVector);

			void _place (curitiba::scene::SceneObject *aSceneObject);

			void getMaterialNames(std::set<std::string> *nameList);

		protected:
			OctreeByMatNode *m_pOctreeRootNode;
			std::string m_Name;
			std::vector<curitiba::scene::SceneObject*> m_vReturnVector;
			
		private:
			void _transformSceneObjects (std::vector<SceneObject*> &sceneObjects);
			curitiba::geometry::BoundingBox _calculateBoundingBox (std::vector<SceneObject*> &sceneObjects);


		};
	};
};

#endif // COCTREE_H
