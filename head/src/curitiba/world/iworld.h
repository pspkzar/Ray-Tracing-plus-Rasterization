#ifndef IWORLD_H
#define IWORLD_H

#include <curitiba/scene/iscene.h>
#include <curitiba/math/vec3.h>

namespace curitiba
{
	namespace world
	{
		class IWorld
		{
		public:
		        
		    virtual ~IWorld() {};
		  
	        virtual void update (void) = 0;
			virtual void build (void) = 0;
			virtual void setScene (curitiba::scene::IScene *aScene) = 0;

			virtual void _add (float mass, curitiba::scene::SceneObject *aObject, std::string name, curitiba::math::vec3 aVec) = 0;
			virtual void setKinematic (std::string name) = 0;
			virtual void setDynamic (std::string name) = 0;

			virtual void disableObject (std::string name) = 0;
			virtual void enableObject (std::string name) = 0;


			virtual void setVelocity (std::string name, curitiba::math::vec3 vel) = 0;
		};
	};
};
#endif
