#ifndef AXIS_H
#define AXIS_H

#include <curitiba/geometry/mesh.h>
#include <curitiba/geometry/primitive.h>

namespace curitiba
{
	namespace geometry
	{
		class Axis : public Primitive
		{
		public:

			friend class curitiba::resource::ResourceManager;
			~Axis(void);


			static const std::string FloatParamNames[];
			
			typedef enum {COUNT_FLOATPARAMS} FloatParams;
			
			//void setParam(unsigned int, float value);
			//float getParamf(unsigned int param);
			//const std::string &getParamfName(unsigned int i);
			void build();

			float getParamf(unsigned int param) {return 0.0f;};

			virtual unsigned int translate(const std::string &name);

		private:
			
			std::vector<float> m_Floats;

		protected:
			Axis();

		};
	};
};
#endif
