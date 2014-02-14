#ifndef IBOUNDINGVOLUME_H
#define IBOUNDINGVOLUME_H

#include <vector>

#include <curitiba/math/vec3.h>
#include <curitiba/render/vertexdata.h>
#include <curitiba/math/itransform.h>

using namespace curitiba::render;
namespace curitiba
{
	namespace geometry
	{

		class IBoundingVolume
		{
		protected:
			enum BoundingVolumeKind {
			  BOX,
			  SPHERE
			};
        public:
			// defines a bounding volume based on two points
			virtual void set(curitiba::math::vec3 a, curitiba::math::vec3 b) = 0;
			virtual void calculate (const std::vector<VertexData::Attr> &vertices) = 0;
			virtual void setTransform (curitiba::math::ITransform &aTransform) = 0;
			virtual bool intersect (const IBoundingVolume *volume) = 0;
			virtual void compound (const IBoundingVolume *volume) = 0;
			virtual bool isA (BoundingVolumeKind kind) const = 0; /***MARK***/
			virtual std::string getType (void) const = 0;
			virtual std::vector<curitiba::math::vec3>& getPoints (void) const = 0;

			virtual const curitiba::math::vec3& getMin (void) const = 0;
			virtual const curitiba::math::vec3& getMax (void) const = 0;
			virtual const curitiba::math::vec3& getCenter (void) const = 0;

			virtual ~IBoundingVolume () {}
		};
	};
};
#endif
