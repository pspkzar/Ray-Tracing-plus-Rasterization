#include <curitiba/event/interpolatorFactory.h>
#include <curitiba/event/positionInterpolator.h>

using namespace curitiba::event_;
using namespace std;

curitiba::event_::Interpolator* 
InterpolatorFactory::create (std::string type)
{
	if ("PositionInterpolator" == type) {
		return new curitiba::event_::PositionInterpolator;
	}

	return 0;
}