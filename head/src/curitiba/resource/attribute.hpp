#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <map>
#include <string>
#include <vector>
#include <assert.h>

#include "curitiba/math/vec4.h"

using namespace curitiba::math;

namespace curitiba {

	namespace resource {
	
		class AttribSet;
		class AttribSetFloat;
		class AttribSetVec4;

		class Attribute {
		
		public:
			friend class AttribSetFloat;
			friend class AttribSetVec4;
			friend class AttribSet;

			typedef enum {
				FLOAT,
				VEC2, VEC3, VEC4,
				INT,
				IVEC2, IVEC3, IVEC4,
				MAT2, MAT3, MAT4
			}enumType;

			Attribute(){};

			Attribute(int id, bool flag): mId(id),mReadOnlyFlag(flag),
					mListDefined(false), mRangeDefined(false) {}; 
			~Attribute() {};

			bool getRangeDefined() { return mRangeDefined; };
			bool getListDefined() { return mListDefined; };

		protected:
			int mId;
			bool mReadOnlyFlag;
			bool mListDefined;
			bool mRangeDefined;

		};


		// -------------------------------------------------------------------------------------------
		//    FLOAT
		// -------------------------------------------------------------------------------------------

		class AttribFloat: public Attribute {
		
		public:
				
			friend class AttribSetFloat;


			AttribFloat() {};
			AttribFloat(int id, float value, bool readOnlyFlag = false): 
				Attribute(id, readOnlyFlag),
				mValue(value)
				{};

			~AttribFloat() {};


			//void set(float val) {
			//
			//	assert(!mReadOnlyFlag && (!mRangeDefined || (val >= mRangeMin && val <= mRangeMax)));

			//	mValue = val;
			//};

			float get() {
				return(mValue);
			}

			void setRange(float min, float max) {

				mRangeMin = min;
				mRangeMax = max;
				mRangeDefined = true;
			};

		protected:

			float mValue;
			float mRangeMin, mRangeMax;
		};

		// -------------------------------------------------------------------------------------------
		//    VEC4
		// -------------------------------------------------------------------------------------------

		class AttribVec4: public Attribute {
		
		public:
				
			friend class AttribSetVec4;

			AttribVec4() {};
			AttribVec4(int id, float x, float y, float z ,float w, bool readOnlyFlag = false): 
				Attribute(id, readOnlyFlag),
				mValue(vec4(x,y,z,x))
				{};

			~AttribVec4() {};



			void set(vec4 &f) {
			
				assert(!mReadOnlyFlag && (!mRangeDefined || (f.x >= mRangeMin && f.x <= mRangeMax
														&& f.y >= mRangeMin && f.y <= mRangeMax
														&& f.z >= mRangeMin && f.z <= mRangeMax
														&& f.w >= mRangeMin && f.w <= mRangeMax)));
				mValue.set(f);
			};

			const vec4 &get() {
				return(mValue);
			}

			void setRange(float min, float max) {

				mRangeMin = min;
				mRangeMax = max;
				mRangeDefined = true;
			};

		protected:

			vec4 mValue;
			float mRangeMin, mRangeMax;
		};

		// -------------------------------------------------------------------------------------------
		//    Attribute Set Float
		// -------------------------------------------------------------------------------------------



		class AttribSetFloat {

		public:
			AttribSetFloat() {};
			~AttribSetFloat() {};

			void add(AttribFloat a) {
			
				mAttributes[a.mId] = a;
			};

			void set(int id, float val) {
			
				assert(mAttributes.find(id) != mAttributes.end());
				mAttributes[id].mValue = val;
			};

			float get(int id) {

				assert(mAttributes.find(id) != mAttributes.end());
				return(mAttributes[id].mValue);

			};

		protected:
			std::map<int, AttribFloat> mAttributes;
		};


		// -------------------------------------------------------------------------------------------
		//    Attribute Set Vec4
		// -------------------------------------------------------------------------------------------

		class AttribSetVec4 {

		public:
			AttribSetVec4() {};
			~AttribSetVec4() {};

			void add(AttribVec4 a) {
			
				mAttributes[a.mId] = a;
			};

			void set(int id, float x, float y, float z, float w) {
			
				assert(mAttributes.find(id) != mAttributes.end());
				mAttributes[id].set(vec4(x,y,z,w));
			};


			void set(int id, float *f) {
			
				assert(mAttributes.find(id) != mAttributes.end());
				mAttributes[id].set(vec4(f[0],f[1],f[2],f[3]));
			};

			void set(int id, const vec4 &v) {
			
				assert(mAttributes.find(id) != mAttributes.end());
				mAttributes[id].set(vec4(v.x, v.y, v.z, v.w));
			};

			vec4 &get(int id) {

				assert(mAttributes.find(id) != mAttributes.end());
				return(mAttributes[id].mValue);

			};

		protected:
			std::map<int, AttribVec4> mAttributes;
		};

	};
};


#endif