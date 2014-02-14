#ifndef LINEARANIMATION_H
#define LINEARANIMATION_H

#include <curitiba/animation/ianimation.h>

#include <curitiba/scene/sceneobject.h>
#include <curitiba/math/vec3.h>

namespace curitiba
{
	namespace animation
	{
		class LinearAnimation : public IAnimation
		{
		public:
			LinearAnimation (curitiba::scene::ISceneObject *aObject, curitiba::math::vec3 start, curitiba::math::vec3 end);
			LinearAnimation (curitiba::scene::ISceneObject *aObject, curitiba::math::vec3 end);

			void step (float deltaT);
			bool isFinished (void);

			~LinearAnimation(void);
		private:
			curitiba::scene::ISceneObject *m_SceneObject;
			curitiba::math::vec3 m_StartPos;
			curitiba::math::vec3 m_LineVector;
			curitiba::math::vec3 m_CurrentPos;

			float m_LocalTime;
		};
	};
};

#endif //LINEARANIMATION_H
