#ifndef OPTIXGEOMETRY_H
#define OPTIXGEOMETRY_H

#include <map>
#include <string>

#include <optix.h>
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>

#include <curitiba/render/optix/optixMaterialLib.h>
#include <curitiba/render/optix/optixBufferLib.h>

namespace curitiba {
  namespace render {
   namespace optixRender {
		
		
	class OptixGeometry {

	public:

		OptixGeometry();
		void setMaterialLib(OptixMaterialLib *oml);
		void setContext(optix::Context &c);
		void setBufferLib(OptixBufferLib *obl);

		void addVertexAttribute(unsigned int attr);

		void addSceneObject(int id, std::map<std::string, curitiba::material::MaterialID> & materialMap);

		void buildGeometryGroup();
		optix::GeometryGroup &getGeometryGroup();

		void setGeometryIntersectProc(std::string, std::string);
		void setBoundingBoxProc(std::string, std::string);

	private:

		optix::Context m_Context;
		optix::GeometryGroup m_GeomGroup;
		OptixBufferLib *m_BufferLib;
		OptixMaterialLib *m_MaterialLib;
		std::vector<bool> m_VertexAttributes;
		optix::Program m_GeomIntersect, m_BoundingBox;
		std::vector<optix::GeometryInstance >  m_GeomInstances;
		
	};
   };
  };
};


#endif