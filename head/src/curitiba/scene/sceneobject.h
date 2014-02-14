#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <curitiba/event/ilistener.h>
#include <curitiba/render/irenderable.h>
#include <curitiba/geometry/iboundingvolume.h>
#include <curitiba/math/itransform.h>
#include <curitiba/scene/sceneobjectfactory.h>

namespace curitiba
{
	namespace scene
	{
		class SceneObject : public curitiba::event_::IListener
		{
		public:
			friend class curitiba::scene::SceneObjectFactory;

			static void ResetCounter();
			static unsigned int Counter;

			virtual std::string getType (void);

			virtual int getId ();
			virtual void setId (int id);

			virtual std::string& getName ();
			virtual void setName (const std::string &name);

			virtual void unitize(float min, float max);

			virtual bool isStatic();
			virtual void setStaticCondition(bool aCondition);

			virtual const curitiba::geometry::IBoundingVolume* getBoundingVolume();
			virtual void setBoundingVolume (curitiba::geometry::IBoundingVolume *b);

			virtual const curitiba::math::ITransform& getTransform();
			virtual void setTransform (curitiba::math::ITransform *t);
			virtual void burnTransform (void);
			virtual curitiba::math::ITransform *_getTransformPtr (void);
			virtual void updateGlobalTransform(curitiba::math::ITransform *m_Transform);
			
			virtual curitiba::render::IRenderable& getRenderable (void);
			virtual curitiba::render::IRenderable* _getRenderablePtr (void);
			virtual void setRenderable (curitiba::render::IRenderable *renderable);

			virtual void writeSpecificData (std::fstream &f);
			virtual void readSpecificData (std::fstream &f);

			void prepareTriangleIDs(bool ids);

			virtual ~SceneObject(void);


		protected:

			SceneObject (void);
			int m_Id;
			std::string m_Name;
			bool m_StaticCondition;
			curitiba::render::IRenderable *m_Renderable;
			curitiba::geometry::IBoundingVolume *m_BoundingVolume;
			curitiba::math::ITransform *m_Transform, *m_GlobalTransform, *m_ResultTransform;

			void calculateBoundingVolume (void);
		};
	};
};

#endif // SCENEOBJECT_H
