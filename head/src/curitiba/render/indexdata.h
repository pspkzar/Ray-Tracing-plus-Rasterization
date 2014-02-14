#ifndef INDEXDATA_H
#define INDEXDATA_H

#include <curitiba/math/vec4.h>
#include <curitiba/render/vertexdata.h>

#include <map>
#include <vector>

using namespace curitiba::math;

namespace curitiba
{
	namespace render
	{
		class IndexData
		{
		public:

			static std::vector<unsigned int> NoIndexData;
		
			static IndexData* create (void);

			virtual ~IndexData(void);

			void offsetIndices (int amount);
			virtual std::vector<unsigned int>& getIndexData (void);
			void setIndexData (std::vector<unsigned int>* indexData);
			unsigned int getIndexSize (void);

			int add (IndexData &anIndexData);

			virtual bool compile (VertexData &v) = 0;
			virtual void resetCompilationFlag() = 0;
			virtual void bind (void) = 0;
			virtual void unbind (void) = 0;
			virtual bool isCompiled() = 0;

			virtual std::vector<unsigned int>& _getReallyIndexData (void) = 0;
			virtual unsigned int getBufferID() = 0;

		protected:
			IndexData(void);
			
			std::vector<unsigned int>* m_InternalIndexArray;
			unsigned int m_IndexSize;
		};
	};
};




#endif //INDEXDATA_H
