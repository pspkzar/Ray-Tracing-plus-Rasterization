#ifndef BULLETWORLD_H
#define BULLETWORLD_H

#include <btBulletDynamicsCommon.h>
#include <curitiba/world/iworld.h>
#include <curitiba/scene/iscene.h>

namespace curitiba
{
	namespace world
	{
		class BulletWorld :
			public curitiba::world::IWorld
		{
		private:
			static const int maxProxies = 32766;

			curitiba::scene::IScene *m_pScene;
			btDynamicsWorld *m_pDynamicsWorld;

			std::map <std::string, btRigidBody*> m_RigidBodies;

		public:
			BulletWorld(void);

			void update (void);
			void build (void);
			void setScene (curitiba::scene::IScene *aScene);

			void _add (float mass, curitiba::scene::SceneObject *aObject, std::string name, curitiba::math::vec3 aVec);
			void setKinematic (std::string name);
			void setDynamic (std::string name);

			void disableObject (std::string name);
			void enableObject (std::string name);

			void setVelocity (std::string name, curitiba::math::vec3 vel);
		
		public:
			~BulletWorld(void);
		};
	};
};
#endif
