#ifndef CURITIBABULLETMOTIONSTATE_H
#define CURITIBABULLETMOTIONSTATE_H

#include <btBulletDynamicsCommon.h>

#include <curitiba/scene/sceneobject.h>

namespace curitiba
{
	namespace world
	{

		class CuritibaBulletMotionState :
			public btMotionState
		{
		private:
			curitiba::scene::SceneObject *m_SceneObject;
			curitiba::math::ITransform *t;

		public:
			CuritibaBulletMotionState(curitiba::scene::SceneObject *aObject);
			~CuritibaBulletMotionState(void);

			/*btMotionState interface*/
			void getWorldTransform (btTransform &worldTrans) const;
			void setWorldTransform (const btTransform &worldTrans);
		};
	};
};

#endif //CURITIBABULLETMOTIONSTATE_H
