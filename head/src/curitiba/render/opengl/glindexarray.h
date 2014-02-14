#ifndef GLINDEXARRAY_H
#define GLINDEXARRAY_H

#include <curitiba/render/indexdata.h>

#include <GL/glew.h>
#include <GL/gl.h>

namespace curitiba
{
	namespace render
	{
		class GLIndexArray : public IndexData
		{
		private:
			bool m_IsCompiled;
			GLuint m_GLBuffer;
			GLuint m_VAO;

		public:
			GLIndexArray(void);

			std::vector<unsigned int>& getIndexData (void);
			virtual bool compile (VertexData &v);
			virtual void resetCompilationFlag();
			virtual void bind (void);
			virtual void unbind (void);
			virtual bool isCompiled();

			virtual std::vector<unsigned int>& _getReallyIndexData (void);
			virtual unsigned int getBufferID();
			~GLIndexArray(void);

		};
	};
};

#endif //GLVERTEXARRAY_H
