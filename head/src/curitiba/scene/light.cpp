#include <curitiba/scene/light.h>
#include <curitiba/errors.h>



const std::string Light::Float4String[] = {"POSITION","DIRECTION",
					"NORMALIZED_DIRECTION", "DIFFUSE", "AMBIENT", "SPECULAR"};

const std::string Light::FloatString[] = {"SPOT_EXPONENT","SPOT_CUTOFF",
					"CONSTANT_ATT", "LINEAR_ATT", "QUADRATIC_ATT"};

const std::string Light::EnumString[] = {"TYPE"};

const std::string Light::BoolString[] = {"ENABLED"};

//using namespace curitiba::math;

Light::Light (std::string &name) :

	m_FloatProps(COUNT_FLOATPROPERTY),
	m_Float4Props(COUNT_FLOAT4PROPERTY),
	m_BoolProps(COUNT_BOOLPROPERTY),
	m_EnumProps(COUNT_ENUMPROPERTY),
	m_Id (-1)
{
	m_Name = name;
	setDefault();
}


Light::~Light(void)
{
}


void
Light::setDefault()
{
	m_FloatProps[SPOT_EXPONENT] = 0.0f;
	m_FloatProps[SPOT_CUTOFF] = 180.0f;
	m_FloatProps[CONSTANT_ATT] = 1.0f;
	m_FloatProps[LINEAR_ATT] = 0.0f;
	m_FloatProps[QUADRATIC_ATT] = 0.0f;

	m_Float4Props[POSITION].set(0.0f, 0.0f, 0.0f, 1.0f);
	m_Float4Props[DIRECTION].set(0.0f, 0.0f, 1.0f, 0.0f);
	m_Float4Props[DIFFUSE].set(1.0f, 1.0f, 1.0f, 1.0f);
	m_Float4Props[AMBIENT].set(0.2f, 0.2f, 0.2f, 1.0f);
	m_Float4Props[SPECULAR].set(0.0f, 0.0f, 0.0f, 0.0f);

	m_BoolProps[ENABLED] = true;
	m_EnumProps[TYPE] = DIRECTIONAL;
}


void
Light::init (curitiba::math::vec3 position,
				  curitiba::math::vec3 direction,
				  curitiba::math::vec4 color, int enabled, LightType type)
{
	setDefault();
	m_Float4Props[POSITION].set(position.x, position.y, position.z, 1.0f);
	m_Float4Props[DIRECTION].set(direction.x, direction.y, direction.z, 0.0f);
	m_Float4Props[NORMALIZED_DIRECTION].set(direction.x, direction.y, direction.z, 0.0f);
	m_Float4Props[NORMALIZED_DIRECTION].normalize();
	m_Float4Props[DIFFUSE].set (color.x, color.y, color.z, color.w);
	m_BoolProps[ENABLED] = (enabled!=0);
	m_EnumProps[TYPE] = type;
}



void 
Light::setId (int id)
{
	m_Id = id;
}

int
Light::getId (void)
{
	return m_Id;
}


std::string
Light::getType() {

	return("LIGHT");
}


void
Light::setProp(FloatProperty prop, float value) {

	float final = value;

	switch (prop) {
		case SPOT_EXPONENT:
			if (value < 0.0f || value > 128.0f)
				final = 0.0f;
			break;
		case SPOT_CUTOFF:
			if (value != 180.0f || value < 0.0f || value > 90.0f)
				final = 180.0f;
			break;
		case CONSTANT_ATT:
		case LINEAR_ATT:
		case QUADRATIC_ATT:
			if (value < 0)
				final = 0;
			break;
	}
	m_FloatProps[prop] = final;
}



void
Light::setProp(Float4Property prop, float x, float y, float z, float w){

	vec4 v;

	v.set(x,y,z,w);

	switch(prop) {
		case POSITION:
			v.w = 1;
			break;
		case DIRECTION:
			v.w = 0;
//			m_Float4Props[DIRECTION].set(v);
			m_Float4Props[NORMALIZED_DIRECTION].set(v);
			m_Float4Props[NORMALIZED_DIRECTION].normalize();
			break;
		case NORMALIZED_DIRECTION:
			v.w = 0;
			v.normalize();
			m_Float4Props[DIRECTION].set(v);
//			m_Float4Props[NORMALIZED_DIRECTION].set(v);
			break;
	}
	m_Float4Props[prop].set(v);
}

void
Light::setProp(Float4Property prop, const vec4& values){

	setProp(prop, values.x, values.y, values.z, values.w);
}

void
Light::setProp(BoolProperty prop, bool value){

	m_BoolProps[prop] = value;
}



void
Light::setProp(EnumProperty prop, int value){

	if (prop == TYPE && value > COUNT_LIGHTTYPE)
			
		CURITIBA_THROW("Invalid Light Type Value %d", value);

	m_EnumProps[prop] = value;
}


float
Light::getPropf(FloatProperty prop) {

	return m_FloatProps[prop];
}

const vec4& 
Light::getPropfv(Float4Property prop) {

	return m_Float4Props[prop];
}

bool 
Light::getPropb(BoolProperty prop) {

	return m_BoolProps[prop];
}

int 
Light::getPrope(EnumProperty prop) {

	return m_EnumProps[prop];
}

void 
Light::getPropTypeAndId(std::string &s, curitiba::Enums::DataType *dt , int *id) 
{
	int i;

	for (i = 0; i < COUNT_BOOLPROPERTY; i++) {

		if (s == BoolString[i]) {
		
			*id = i;
			*dt = Enums::BOOL;
			return;
		}
	}

	for (i = 0; i < COUNT_FLOATPROPERTY; i++) {

		if (s == FloatString[i]) {
		
			*id = i;
			*dt = Enums::FLOAT;
			return;
		}
	}

	for (i = 0; i < COUNT_FLOAT4PROPERTY; i++) {

		if (s == Float4String[i]) {
		
			*id = i;
			*dt = Enums::VEC4;
			return;
		}
	}

	for (i = 0; i < COUNT_ENUMPROPERTY; i++) {

		if (s == EnumString[i]) {
		
			*id = i;
			*dt = Enums::ENUM;
			return;
		}
	}

	CURITIBA_THROW("Invalid Light Property %s", s);

}

			
std::string 
Light::getPropfString(Light::FloatProperty prop) 
{
	return FloatString[prop];
}

			
std::string 
Light::getPropfvString(Light::Float4Property prop)
{
	return Float4String[prop];
}

			
std::string 
Light::getPropbString(Light::BoolProperty prop)
{
	return BoolString[prop];
}

			
std::string 
Light::getPropeString(Light::EnumProperty prop)
{
	return EnumString[prop];
}


//
//void 
//Light::setPosition (curitiba::math::vec3 &position)
//{
//	m_Position.set (position.x, position.y, position.z);
//}
//
//curitiba::math::vec3& 
//Light::getPosition (void)
//{
//	return m_Position;
//}
//
//void 
//Light::setDirection (curitiba::math::vec3 &direction)
//{
//	m_Direction.set (direction.x, direction.y, direction.z);
//}
//
//curitiba::math::vec3& 
//Light::getDirection (void)
//{
//	return m_Direction;
//}
//
//void 
//Light::setColor (curitiba::math::vec4 &color)
//{
//	m_Color.set (color.x, color.y, color.z, color.w);
//}
//
//curitiba::math::vec4& 
//Light::getColor (void)
//{
//	return m_Color;
//}
//
//void Light::setAmbient (curitiba::math::vec4 &color)
//{
//	m_Ambient.set (color.x, color.y, color.z, color.w);
//}
//
//curitiba::math::vec4& 
//Light::getAmbient (void)
//{
//	return m_Ambient;
//}
//
//void Light::setSpecular (curitiba::math::vec4 &color)
//{
//	m_Specular.set (color.x, color.y, color.z, color.w);
//}
//
//curitiba::math::vec4& 
//Light::getSpecular (void)
//{
//	return m_Specular;
//}
//
//			
//void 
//Light::setSpotCutOff(float co) 
//{
//	m_FloatProps[SPOT_CUTOFF] = co;
//}
//			
//float 
//Light::getSpotCutOff() 
//{
//	return m_FloatProps[SPOT_CUTOFF];
//}
//
//
//void 
//Light::setSpotExponent(float exp)
//{
//	m_FloatProps[SPOT_EXPONENT] = exp;
//}
//
//	
//float 
//Light::getSpotExponent()
//{
//	return m_FloatProps[SPOT_EXPONENT];
//}
//
//			
//void 
//Light::setConstantAttenuation(float a)
//{
//	m_ConstantAtt = a;
//}
//
//float 
//Light::getConstantAttenuation()
//{
//	return m_ConstantAtt;
//}
//
//			
//void 
//Light::setLinearAttenuation(float a)
//{
//	m_LinearAtt = a;
//}
//
//			
//float 
//Light::getLinearAttenuation() 
//{
//	return m_LinearAtt;
//}
//
//			
//void 
//Light::setQuadraticAttenuation(float a)
//{
//	m_QuadraticAtt = a;
//}
//
//			
//float 
//Light::getQuadraticAttenuation()
//{
//	return m_QuadraticAtt;
//}
//
//
//Light::LightType
//Light::getLightType (void)
//{
//	return m_LightType;
//}
//
//void 
//Light::setLightType(LightType aType) 
//{
//	m_LightType = aType;
//}
//
//void
//Light::enable(bool value) {
//
//	m_Enabled = value;
//}
//
//bool
//Light::isEnabled() {
//
//	return(m_Enabled);
//}
//
//Light::Light (std::string name, curitiba::math::vec3 position,
//				  curitiba::math::vec3 direction,
//				  curitiba::math::vec4 color, LightType type) :
//	m_Name (name),
//	m_Position (position),
//	m_Direction (direction),
//	m_Color (color),
//	m_Id (-1),
//	m_LightType (type)
//{
//}

//void
//Light::init (curitiba::math::vec3 position,
//				  curitiba::math::vec3 direction,
//				  curitiba::math::vec4 color, int enabled, LightType type)
//{
//	m_Position.set (position.x, position.y, position.z);
//	m_Direction.set (direction.x, direction.y, direction.z);
//	m_Color.set (color.x, color.y, color.z, color.w);
//	m_Enabled = (enabled!=0);
//	m_LightType = type;
//}
//void 
//Light::setName (std::string name)
//{
//	m_Name = name;
//}
//
//std::string& 
//Light::getName (void)
//{
//	return m_Name;
//}
