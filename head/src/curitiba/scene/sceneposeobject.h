#ifndef SCENEPOSEOBJECT_H
#define SCENEPOSEOBJECT_H

#include <curitiba/scene/sceneobject.h>

namespace curitiba
{
	namespace scene
	{
		class ScenePoseObject : public SceneObject
		{
		public:
			ScenePoseObject (void);
			virtual ~ScenePoseObject(void);

			virtual void unitize(float min, float max);
			virtual bool isStatic();
			virtual void setStaticCondition(bool aCondition);

			virtual const curitiba::geometry::IBoundingVolume* getBoundingVolume();
			virtual void setBoundingVolume (curitiba::geometry::IBoundingVolume *b);
			virtual const curitiba::math::ITransform& getTransform();
			//virtual void setTransform (curitiba::math::ITransform *t);

			virtual void burnTransform (void);
			
			virtual void writeSpecificData (std::fstream &f);
			virtual void readSpecificData (std::fstream &f);

			virtual std::string getType (void);

		protected:
			void calculateBoundingVolume (void);
		};
	};
};

#endif // SCENEOBJECT_H
