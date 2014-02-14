#ifndef OPTIXTEXTURELIB_H
#define OPTIXTEXTURELIB_H

#include <map>
#include <string>

#include <optix.h>
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>

#include <curitiba/material/material.h>

namespace curitiba {
  namespace render {
   namespace optixRender {
		
		
	class OptixTextureLib {

	public:

		OptixTextureLib() {};
		void setContext(optix::Context &aContext);
		unsigned int addTextures(curitiba::material::Material *m);
		void addTexture(unsigned int glID);
		optix::TextureSampler &getTexture(unsigned int GLID);

		void applyTextures(optix::GeometryInstance, curitiba::material::Material *m);

	private:
		
		std::map<unsigned int, optix::TextureSampler> m_TextureLib;
		optix::Context m_Context;

		int translateWrapModeToOptix(int mode);
		int translateFilterModeToOptix(int mode);
	};
   };
  };
};


#endif