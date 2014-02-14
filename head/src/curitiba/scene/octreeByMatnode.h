#ifndef OCTREEBYMATNODE_H
#define OCTREEBYMATNODE_H

#include <vector>

#include <curitiba/scene/sceneobject.h>
#include <curitiba/scene/camera.h>
#include <curitiba/material/materialgroup.h>
#include <curitiba/render/irenderable.h>
#include <curitiba/geometry/boundingbox.h>
#include <curitiba/geometry/mesh.h>
#include <curitiba/geometry/frustum.h>
#include <curitiba/math/vec3.h>


namespace curitiba {
	namespace loader {
		class CBOLoader;
	};
};

namespace curitiba
{
	namespace scene
	{

		class OctreeByMatNode //: public SceneObject
		{
			friend class curitiba::loader::CBOLoader;
			friend class OctreeByMat;
			
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

			OctreeByMatNode *m_pParent;
			OctreeByMatNode *m_pChilds[8];

			int m_ChildCount;
			bool m_Divided;
			int m_NodeId;
			int m_NodeDepth;

			std::map<std::string, curitiba::scene::SceneObject *> m_pLocalMeshes;

		public:
			OctreeByMatNode ();
			
			OctreeByMatNode (OctreeByMatNode *parent, vec3 bbMin, vec3 bbMax, int nodeId = 0, int nodeDepth = 0);
			void updateNodeTransform(curitiba::math::ITransform *t);
			//void addRenderable (curitiba::render::IRenderable *aRenderable);
			void setRenderable (curitiba::render::IRenderable *renderable);

			void getMaterialNames(std::set<std::string> *nameList);

			virtual std::string getType (void);
			void setName(std::string name);
			std::string getName();

			virtual void writeSpecificData (std::fstream &f);
			virtual void readSpecificData (std::fstream &f);
			void tightBoundingVolume();
			void unitize(float min, float max);
			
			virtual ~OctreeByMatNode(void);


		protected:
			std::string m_Name;
			void _compile (void);
			void _findVisibleSceneObjects (std::vector<curitiba::scene::SceneObject*> &m_vReturnVector,
																curitiba::geometry::Frustum &aFrustum, 
																curitiba::scene::Camera &aCamera,
																bool conservative = false);
			void getAllSceneObjects (std::vector<curitiba::scene::SceneObject*> &m_vReturnVector);

			curitiba::geometry::BoundingBox m_BoundingVolume, m_TightBoundingVolume;

			OctreeByMatNode* _getChild (int i);
			void _setParent (OctreeByMatNode *parent);
			void _setChild (int i, OctreeByMatNode *aNode);
			int _getChildCount (void);

		private:
			int _octantFor (VertexData::Attr& v);
			OctreeByMatNode* _createChild (int octant);
			std::string _genOctName (void);
			void _split();
			void _unifyLocalMeshes();

		};
	};
};
#endif //COCTREENODE_H
