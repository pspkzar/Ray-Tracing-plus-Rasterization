#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <curitiba/math/vec3.h>
#include <curitiba/math/vec4.h>
#include <curitiba/event/ilistener.h>

#include <map>

using namespace curitiba::math;


namespace curitiba
{
	namespace event_
	{
		class Interpolator: public IListener
		{

		public:
			typedef enum {BOOL, FLOAT, VEC3, COUNT_PROPTYPE} PropTypes;

		protected:
			curitiba::math::vec3 emptyVec3;
			std::string emptyString;
			std::string m_Name;

			//std::vector<vec4> m_Values;
			std::map<float, vec4> m_KeyFrames;

		public:
			Interpolator(void): /*m_Values(), m_Keys(), m_Key(0)*/ m_KeyFrames() {};
			~Interpolator(void){};

			void setName(std::string &name) {m_Name = name;};
			std::string &getName() {return m_Name;}

			//virtual void addValue(vec4 &value) {m_Values.push_back(vec4(value.x, value.y, value.z, value.w));};

			//virtual void addKey(float key) {m_Keys.push_back(key);};
			virtual void addKeyFrame(float key, vec4 &value) {m_KeyFrames[key] = value;};

			//virtual void setPositionInterpolator(std::string name, std::vector<float> key, std::vector<vec3> keyValue){};
			//virtual void init(std::string name, char *ckey, char *ckeyValue){};

			virtual const std::string &getBoolPropNames(unsigned int i) {return emptyString;};
			virtual const std::string &getFloatPropNames(unsigned int i) {return emptyString;};
			virtual const std::string &getVec3PropNames(unsigned int i) {return emptyString;};


			virtual void setBool(unsigned int prop, bool value) {};
			virtual void setVec3(unsigned int prop, curitiba::math::vec3 &value) {};
			virtual void setFloat(unsigned int prop, float value) {};

			virtual float getFloat(unsigned int prop) {return 0.0f;};
			virtual bool getBool(unsigned int prop) {return false;};
			virtual curitiba::math::vec3 &getVec3(unsigned int prop) {return emptyVec3;}

			virtual unsigned int getFloatPropCount() {return 0;};
			virtual unsigned int getBoolPropCount() {return 0;};
			virtual unsigned int getVec3PropCount() {return 0;};

		};

	};
};

#endif