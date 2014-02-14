#ifndef POSITIONINTERPOLATOR_H
#define POSITIONINTERPOLATOR_H

#include <curitiba/event/eventvec4.h>
#include <curitiba/event/interpolator.h>
#include <curitiba/math/vec4.h>


namespace curitiba
{
	namespace event_
	{
			class PositionInterpolator: public Interpolator
			{
			private:
				float fraction;
				curitiba::event_::EventVec4 e;

			public:
				PositionInterpolator(const PositionInterpolator &c);
				PositionInterpolator(void);
				~PositionInterpolator(void);

				void removePositionListener(void);
				void addPositionListener(void);
				void eventReceived(const std::string &sender, const std::string &eventType, curitiba::event_::IEventData *evt);
			};
	};
};
#endif