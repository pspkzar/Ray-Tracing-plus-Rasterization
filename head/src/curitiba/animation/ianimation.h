#ifndef IANIMATION_H
#define IANIMATION_H

namespace curitiba 
{
	namespace animation
	{

		class IAnimation
		{
		public:
			virtual void step (float deltaT) = 0;
			virtual bool isFinished (void) = 0;

			virtual ~IAnimation(void) {};
		};
	};
};
#endif //IANIMATION_H
