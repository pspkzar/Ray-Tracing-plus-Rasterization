#ifndef POSEOFFSET_H
#define POSEOFFSET_H

#include <curitiba/math/vec3.h>
#include <vector>

using namespace curitiba::math;


namespace curitiba
{
	namespace geometry 
	{
		class PoseOffset  
		{
			private:
				std::vector<vec3> m_vOffset;
				int m_Size;

				PoseOffset(void);

			public:
				PoseOffset(unsigned int aSize);
				void addPoseOffset(int aIndex, float x, float y, float z);
				std::vector<vec3> getOffsets();
				~PoseOffset();

		};
	};

}

#endif

