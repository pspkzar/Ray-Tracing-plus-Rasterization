#ifndef OCTREESCENE_H
#define OCTREESCENE_H

#include <curitiba/scene/iscenepartitioned.h>
#include <curitiba/scene/octree.h>
#include <curitiba/geometry/frustum.h>
#include <curitiba/geometry/boundingbox.h>

namespace curitiba {

	namespace scene {

		class OctreeScene : public IScenePartitioned
		{
		private:
			std::vector<SceneObject*> m_vReturnVector;
			std::vector<SceneObject*> m_SceneObjects;

			Octree *m_pGeometry;

			//bool m_Visible;

			curitiba::geometry::BoundingBox m_BoundingBox;

		protected:
			void updateSceneObjectTransforms();

		public:
			OctreeScene (void);
			~OctreeScene (void);

			void clear();

			virtual void eventReceived(const std::string &sender, const std::string &eventType, curitiba::event_::IEventData *evt);

			virtual void build (void);
			
			virtual void compile (void);

			virtual curitiba::geometry::IBoundingVolume& getBoundingVolume (void);

			virtual void unitize();

			virtual void add (curitiba::scene::SceneObject *aSceneObject);
			
			virtual std::vector <SceneObject*>& findVisibleSceneObjects 
																(curitiba::geometry::Frustum &aFrustum, 
																Camera &aCamera,
																bool conservative = false);
			virtual std::vector<SceneObject*>& getAllObjects ();

			virtual curitiba::scene::SceneObject* getSceneObject (std::string name);
			virtual curitiba::scene::SceneObject* getSceneObject (int index);

			virtual void getMaterialNames(std::set<std::string> *nameList);

			virtual curitiba::math::ITransform *getTransform();
			virtual void setTransform(curitiba::math::ITransform *t);
			virtual void transform(curitiba::math::ITransform *t);


			virtual std::string getType (void);

			/*
			 * Statistical information
			 */

			int getNumTriangles() { return 0; };
			int getNumVertices() { return 0; };
		
			// By material
			int getNumTrianglesMat(int i) { return 0; };
			int getNumVerticesMat(int i) { return 0; ;}

			// By Object
			int getNumTrianglesObject(int i) { return 0; };
			int getNumVerticesObject(int i) { return 0; };

			//virtual void scale(float factor) = 0;
			//virtual void translate(float x, float y, float z);
			//virtual void rotate(float ang, float ax, float ay, float az) = 0;
			//void show (void);			
			//void hide (void);
			//bool isVisible (void);

		};
	};
};

#endif
