#ifndef CAMERA_H
#define CAMERA_H

#include <string>

#include <curitiba/scene/sceneobject.h>
#include <curitiba/math/simpletransform.h>
#include <curitiba/math/vec4.h>
#include <curitiba/math/spherical.h>
#include <curitiba/render/viewport.h>
#include <curitiba/enums.h>
#include <curitiba/resource/attribute.hpp>

#include <curitiba/event/EventVec3.h>

using namespace curitiba::math;
using namespace curitiba::resource;

namespace curitiba
{
	namespace scene
	{

		class Camera : public SceneObject
		{
		public:

			typedef enum {
				ORTHO,
				PERSPECTIVE
			} CameraType;

			typedef enum { POSITION, VIEW_VEC, NORMALIZED_VIEW_VEC, 
				UP_VEC, NORMALIZED_UP_VEC, 
				NORMALIZED_RIGHT_VEC, LOOK_AT_POINT, 
				COUNT_FLOAT4PROPERTY} Float4Property;

			typedef enum { FOV, NEARP, FARP, LEFT, RIGHT, TOP, BOTTOM, 
				COUNT_FLOATPROPERTY} FloatProperty;

			typedef enum { VIEW_MATRIX, PROJECTION_MATRIX, VIEW_INVERSE_MATRIX,
				PROJECTION_VIEW_MATRIX, TS05_PVM_MATRIX, COUNT_MAT4PROPERTY } Mat4Property;

			static const std::string Float4String[COUNT_FLOAT4PROPERTY];
			static const std::string FloatString[COUNT_FLOATPROPERTY];
			static const std::string Mat4String[COUNT_MAT4PROPERTY];

			static void getPropTypeAndId(std::string &s, curitiba::Enums::DataType *dt , int *id);

			Camera (const std::string &name);
			virtual ~Camera (void);


			void setProp(Float4Property prop, float r, float g, float b, float a);
			void setProp(FloatProperty prop, float value);

			float getPropf(FloatProperty prop);
			//static const std::string &getPropfString(FloatProperty);

			const mat4 &getPropm4(Mat4Property prop);
			//static const std::string &getPropm4String(Mat4Property prop);

			const vec4 &getPropfv(Float4Property prop);
			//static const std::string &getPropfvString(Float4Property prop);

			void setOrtho (float left, float right, float bottom, float top, float near, float far);
			void setPerspective (float fov, float near, float far);

			void eventReceived(const std::string &sender, const std::string &eventType, curitiba::event_::IEventData *evt);

			// Spherical Coordinates
			void setElevationAngle(float angle);
			void setZXAngle(float angle);
			float getElevationAngle();
			float getZXAngle();
			void setVectorsFromSpherical();

			bool getLookAt();
			void setLookAt(bool flag);


			void setCamera (vec3 position, vec3 view, vec3 up);

			// Projections
			void setProjectionType(CameraType ct);
			unsigned int getProjectionType();

			// Viewport
			curitiba::render::Viewport* getViewport (void);
			void setViewport (curitiba::render::Viewport* aViewport);

			// Adjusts the frustum of the current Camera to include
			// the frustum of the target camera
			// usefull for shadow mapping for instance
			void adjustMatrix (curitiba::scene::Camera *targetCamera);
			// This version considers only a fraction of the target camera
			// the params near and far relate to the targets camera frustum
			void adjustMatrixPlus (float cNear, float cFar, curitiba::scene::Camera *targetCamera);

			// Bounding Volume 	
			virtual const curitiba::geometry::IBoundingVolume* getBoundingVolume();

			// Renderable is the graphic representation of the camera
			// usefull for debug purposes
			curitiba::render::IRenderable& getRenderable();

			// used for Physics
			bool isDynamic();
			void setDynamic(bool value);
			void setPositionOffset (float value);

			//std::string& getName (void);

		protected:

			//AttribSetFloat mFloatAttribs;
			//AttribSetVec4 mVec4Attribs;

			std::vector<vec4> m_Float4Props;
			std::vector<float> m_FloatProps;
			std::vector<SimpleTransform> m_Mat4Props;
			curitiba::event_::EventVec3 m_Event;
			curitiba::render::Viewport *m_pViewport;

			vec3 result;
			// LookAt settings
			bool m_LookAt;
			vec3 m_LookAtPoint;

			// Camera Spherical Coordinates (angles are stored in radians)
			// Spherical(0,0) means Cartesian(0,0,1)
			spherical m_Spherical;

			// Physics
			float m_PositionOffset;
			bool m_IsDynamic; 

			// Projections
			bool m_IsOrtho;

			void updateProjection();

			void setProp(Mat4Property prop, mat4 &mat);
			// Matrices
			void buildViewMatrix (void);
			void buildViewMatrixInverse(void);
			void buildProjectionMatrix();
			void buildProjectionViewMatrix(void);
			void buildTS05PVMMatrix(void);


			// The eight corners of the frustum
			enum {
				TOP_LEFT_NEAR = 0,
				TOP_RIGHT_NEAR,
				BOTTOM_RIGHT_NEAR,
				BOTTOM_LEFT_NEAR,
				TOP_LEFT_FAR,
				TOP_RIGHT_FAR,
				BOTTOM_RIGHT_FAR,
				BOTTOM_LEFT_FAR,
			};		


		};
	};
};

#endif //CAMERA_H
