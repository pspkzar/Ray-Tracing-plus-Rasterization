#ifndef PLANE_H
#define PLANE_H

#include <curitiba/math/vec3.h>

namespace curitiba
{
	namespace math
	{
		class plane
		{
		private:
			vec3 normal;
			float d;

		public:
			plane(void);

			void setCoefficients (float a, float b, float c, float d);
			float distance (vec3 &v);

			const vec3& getNormal (void);
		public:
			~plane(void);
		};
	};
};
#endif //PLANE_H
