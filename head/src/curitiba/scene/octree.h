#ifndef OCTREE_H
#define OCTREE_H

#include <curitiba/scene/octreenode.h>
#include <curitiba/scene/iscene.h>
#include <curitiba/scene/camera.h>
#include <curitiba/math/vec3.h>
#include <curitiba/geometry/boundingbox.h>
#include <curitiba/clogger.h> /***MARK***/

#include <vector>
#include <string>

namespace curitiba
{

	namespace scene
	{

		//class COctreeNode;
		class Octree {

			friend class OctreeNode;
			//friend class boost::serialization::access;

		public:
			Octree();

			virtual ~Octree();

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

		private:
			//void renderOctreeNode (COctreeNode* aNode, IWorld& aWorld, bool testAgainstFrustum);

		private:
			OctreeNode *m_pOctreeRootNode;
			//std::vector<curitiba::material::IMaterialGroup*> m_MaterialGroups;
			std::vector<curitiba::scene::SceneObject*> m_vReturnVector;
			
		private:
			void _transformSceneObjects (std::vector<SceneObject*> &sceneObjects);
			curitiba::geometry::BoundingBox _calculateBoundingBox (std::vector<SceneObject*> &sceneObjects);

			//boot serialization interface
		private:
			template<class Archive>
			void serialize (Archive &ar, const unsigned int version)
			{
			}

		};
	};
};

#endif // COCTREE_H
