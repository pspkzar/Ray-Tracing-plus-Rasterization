#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <vector>

#include <curitiba/geometry/iboundingvolume.h>
#include <curitiba/math/simpletransform.h>
#include <curitiba/math/vec3.h>
#include <curitiba/geometry/bbox.h>

namespace curitiba
{
	namespace geometry
	{
		class BoundingBox :
			public curitiba::geometry::IBoundingVolume
		{
		private:
			enum {
				MIN = 0,
				MAX,
				CENTER
			};
	
			mutable std::vector<curitiba::math::vec3> m_vPoints,m_vLocalPoints; /***MARK***/ //Should not be mutable
			//std::vector<curitiba::math::vec3> m_vLocalPoints;

			SimpleTransform m_GeometryTransform; 

		public:

#ifdef CURITIBA_RENDER_FLAGS
			static curitiba::geometry::BBox *Geometry;
			static curitiba::geometry::BBox *getGeometry();
#endif
			BoundingBox (void);
			BoundingBox (curitiba::math::vec3 min, curitiba::math::vec3 max);
			BoundingBox (const BoundingBox &aBoundingBox);

			void set(curitiba::math::vec3 min, curitiba::math::vec3 max);
			void calculate (const std::vector<VertexData::Attr> &vertices);

			// updates the geometry transform
			void setTransform (curitiba::math::ITransform &aTransform);
			ITransform *getTransform();


			bool intersect (const IBoundingVolume *volume);
			void compound (const IBoundingVolume *volume);
			
			bool isA (BoundingVolumeKind kind) const;
			std::string getType (void) const;
			std::vector<curitiba::math::vec3>& getPoints (void) const;
			std::vector<curitiba::math::vec3>& getNonTransformedPoints (void) const;

			const curitiba::math::vec3& getMin (void) const;
			const curitiba::math::vec3& getMax (void) const;

			const curitiba::math::vec3& getCenter (void) const;
		private:
			void _calculateCenter (void);

		public:
			virtual ~BoundingBox(void);
		};
	};
};

#endif
