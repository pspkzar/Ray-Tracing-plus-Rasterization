#ifndef OCTREENODE_H
#define OCTREENODE_H

#include <vector>

#include <curitiba/scene/sceneobject.h>
#include <curitiba/scene/camera.h>
#include <curitiba/material/materialgroup.h>
#include <curitiba/render/irenderable.h>
#include <curitiba/geometry/boundingbox.h>
#include <curitiba/geometry/mesh.h>
#include <curitiba/geometry/frustum.h>
#include <curitiba/math/vec3.h>

namespace curitiba
{
	namespace scene
	{

		class OctreeNode : public SceneObject
		{
//			friend class boost::serialization::access;
			friend class Octree;
			
		protected:
			
			static const int MAXPRIMITIVES = 25000;

			enum {
			  TOPFRONTLEFT = 0,
			  TOPFRONTRIGHT,
			  TOPBACKLEFT,
			  TOPBACKRIGHT,
			  BOTTOMFRONTLEFT,
			  BOTTOMFRONTRIGHT,
			  BOTTOMBACKLEFT,
			  BOTTOMBACKRIGHT,
			  ROOT
			};

			OctreeNode* m_pParent;
			OctreeNode* m_pChilds[8];

			int m_ChildCount;
			bool m_Divided;
			int m_NodeId;
			int m_NodeDepth;

			curitiba::geometry::Mesh *m_pLocalMesh;

		//	curitiba::geometry::BoundingBox m_BoundingBox;

		public:
			OctreeNode ();
			
			OctreeNode (OctreeNode *parent, curitiba::geometry::IBoundingVolume *boundingBox, int nodeId = 0, int nodeDepth = 0);
			void updateNodeTransform(curitiba::math::ITransform *t);
			//void addRenderable (curitiba::render::IRenderable *aRenderable);
			void setRenderable (curitiba::render::IRenderable *renderable);

			void getMaterialNames(std::set<std::string> *nameList);

			virtual std::string getType (void);

			virtual void writeSpecificData (std::fstream &f);
			virtual void readSpecificData (std::fstream &f);
			void tightBoundingVolume();
			void OctreeNode::unitize(float min, float max);
			
			virtual ~OctreeNode(void);


		protected:
			void _compile (void);
			void _findVisibleSceneObjects (std::vector<curitiba::scene::SceneObject*> &m_vReturnVector,
						curitiba::geometry::Frustum &aFrustum, 
						curitiba::scene::Camera &aCamera,
						bool conservative = false);
			void resetCounter();	
			static int counter;
			

			OctreeNode* _getChild (int i);
			void _setParent (OctreeNode *parent);
			void _setChild (int i, OctreeNode *aNode);
			int _getChildCount (void);

		private:
			int _octantFor (VertexData::Attr& v);
			OctreeNode* _createChild (int octant);
			std::string _genOctName (void);


		};
	};
};
#endif //COCTREENODE_H
