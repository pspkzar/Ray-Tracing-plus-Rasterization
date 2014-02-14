#ifndef LIGHT_H
#define LIGHT_H

#include <curitiba/math/vec3.h>
#include <curitiba/math/vec4.h>
#include <curitiba/scene/sceneobject.h>
#include <curitiba/enums.h>

#include <string>

using namespace curitiba::math;
using namespace curitiba::scene;

namespace curitiba
{
	namespace scene
	{
		class Light: public SceneObject
		{
		public:

			typedef enum {
				DIRECTIONAL,
				POSITIONAL,
				SPOT_LIGHT,
				OMNILIGHT,COUNT_LIGHTTYPE
			} LightType;

			typedef enum {POSITION, DIRECTION, NORMALIZED_DIRECTION, DIFFUSE,
				AMBIENT,SPECULAR,
				COUNT_FLOAT4PROPERTY} Float4Property;
			typedef enum {SPOT_EXPONENT, SPOT_CUTOFF, CONSTANT_ATT, 
				LINEAR_ATT, QUADRATIC_ATT,
				COUNT_FLOATPROPERTY} FloatProperty;
			typedef enum {ENABLED, COUNT_BOOLPROPERTY} BoolProperty;
			typedef enum {TYPE, COUNT_ENUMPROPERTY} EnumProperty;

			static const std::string Float4String[COUNT_FLOAT4PROPERTY];
			static const std::string FloatString[COUNT_FLOATPROPERTY];
			static const std::string BoolString[COUNT_BOOLPROPERTY];
			static const std::string EnumString[COUNT_ENUMPROPERTY];

		protected:
			std::vector<float> m_FloatProps;
			std::vector<vec4> m_Float4Props;
			std::vector<bool> m_BoolProps;
			std::vector<int> m_EnumProps;

			int m_Id;
		
		public:
			static void getPropTypeAndId(std::string &s, curitiba::Enums::DataType *dt , int *id);
			static std::string getPropfString(Light::FloatProperty prop);
			static std::string getPropfvString(Light::Float4Property prop);
			static std::string getPropbString(Light::BoolProperty prop);
			static std::string getPropeString(Light::EnumProperty);

			Light (std::string &name);
			~Light(void);

			void init (curitiba::math::vec3 position,
				  curitiba::math::vec3 direction,
				  curitiba::math::vec4 color, int enabled, LightType type);

			void setDefault();

			void setId (int id);
			int getId (void);


			// returns "LIGHT"
			std::string getType();

			void setProp(Light::FloatProperty prop, float value);
			void setProp(Light::Float4Property prop, float r, float g, float b, float a);
			void setProp(Light::Float4Property prop, const vec4& aVec);
			void setProp(Light::BoolProperty prop, bool value);
			void setProp(Light::EnumProperty, int value);

			float getPropf(Light::FloatProperty prop);
			const vec4 &getPropfv(Light::Float4Property prop);
			bool getPropb(Light::BoolProperty prop);
			int getPrope(Light::EnumProperty);



			//void setName (std::string name);
			//std::string &getName (void);

		//	vec3 m_Position;
		//	vec3 m_Direction;
		//	vec4 m_Color;
		//	vec4 m_Ambient;
		//	vec4 m_Specular;
		//	float m_SpotExponent;
		//	float m_SpotCutOff;
		//	float m_ConstantAtt;
		//	float m_LinearAtt;
		//	float m_QuadraticAtt;


			//Light (std::string name, curitiba::math::vec3 position,
			//			curitiba::math::vec3 direction,
			//			curitiba::math::vec4 color, LightType type);


			//float getPropf(FloatProperty prop);
			//const vec4& getProp4f(Float4Property prop);




			//void enable(bool value); 
			//bool isEnabled(); 

			//void setPosition (curitiba::math::vec3 &position);
			//curitiba::math::vec3& getPosition (void);

			//void setDirection (curitiba::math::vec3 &direction);
			//curitiba::math::vec3& getDirection (void);

			//void setColor (curitiba::math::vec4 &color);
			//curitiba::math::vec4& getColor (void);

			//void setAmbient (curitiba::math::vec4 &color);
			//curitiba::math::vec4& getAmbient (void);

			//void setSpecular (curitiba::math::vec4 &color);
			//curitiba::math::vec4& getSpecular (void);

			//void setSpotCutOff(float co);
			//float getSpotCutOff();

			//void setSpotExponent(float exp);
			//float getSpotExponent();

			//void setConstantAttenuation(float a);
			//float getConstantAttenuation();

			//void setLinearAttenuation(float a);
			//float getLinearAttenuation();

			//void setQuadraticAttenuation(float a);
			//float getQuadraticAttenuation();

			//LightType getLightType (void);
			//void setLightType(LightType aType);
		};
	};
};



#endif
