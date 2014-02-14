#ifndef GLSTATE_H
#define GLSTATE_H

#include <curitiba/render/istate.h>
#include <string>
#include <GL/glew.h>


namespace curitiba
{
	namespace render
	{

		class GlState: public IState {


		public:

			GlState();
			~GlState();

			virtual void set();
			void setDiff(IState *defState, IState *pState);

			//virtual void setDiff(IState *pState);
			//virtual void getCurrent();
			//void resetDiff(OGLState &oState);
			//void combine(OGLState &a,OGLState &b);

		private:
			bool difColor(vec4& a, vec4& b);
			bool difBoolVector(bool* a, bool* b);

			unsigned int translate(CullType aProp);
			unsigned int translate(BlendFunc aProp);
			unsigned int translate(BlendEquation aProp);
			unsigned int translate(FogMode aProp);
			unsigned int translate(FogCoordSrc aProp);
			unsigned int translate(Func aFunc);

			CullType translateCullType(GLint aProp);
			Func translateFunc(GLint aProp);
			BlendFunc translateBlendFunc(GLint aProp);
			BlendEquation translateBlendEquation(GLint aProp);
			FogMode translateFogMode(GLint aProp);
			FogCoordSrc translateFogCoordSrc(GLint aProp);

			float* toFloatPtr(vec4& v);

		};
	};
};

#endif //GLSTATE_H
