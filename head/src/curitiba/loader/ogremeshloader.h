#ifndef OGREMESHLOADER_H
#define OGREMESHLOADER_H

#include <string>
#include <vector>

#include <tinyxml.h>
#include <curitiba/system/fileutil.h>
#include <curitiba/scene/iscene.h>
#include <curitiba/material/materialgroup.h>
#include <curitiba/material/materialLib.h>
#include <curitiba/math/vec3.h>
#include <curitiba/scene/sceneposes.h>
#include <curitiba/scene/sceneskeleton.h>
#include <curitiba/geometry/meshbones.h>

using namespace curitiba::render;
using namespace curitiba::math;
using namespace curitiba::scene;
using namespace curitiba::material;
using namespace curitiba::system;

namespace curitiba
{
	namespace loader
	{
		class OgreMeshLoader
		{
		public:
			static void loadScene (IScene* aScene, std::string file) throw (std::string);
			static std::string m_Path;
			static std::string m_MeshFile;
			static std::string m_SkeletonFile;

		private:

			static std::string m_MeshType;

			typedef enum {
				SIMPLE,
				POSE,
				BONES
			}MeshTypes;

			OgreMeshLoader(void);

			static void loadVertexElement(TiXmlElement *pElemVertexAttrib, vec4 *vertexElem) ;
			static void loadTextureCoordElement(TiXmlElement *pElemVertexAttrib, vec4 *vertexElem); 
			static void loadVertexBuffer(TiXmlElement *pElemVertexBuffer, VertexData &vertexData);
			static void loadSubMeshes (TiXmlHandle hRoot, IScene *scn, IRenderable *m, std::string meshType);
			static void loadGeometry(TiXmlElement *pElem, VertexData &vertexData);
			static void loadFaces(TiXmlElement *pElem, MaterialGroup *mg, unsigned int operationType);
			static IRenderable *loadSharedGeometry (TiXmlHandle hRoot, IScene *scn, std::string meshType);
			static void loadSubMeshNames(TiXmlHandle hRoot, IScene *scn, bool meshSharedGeometry);
			static void loadVertexBuffers(TiXmlElement *pElem, VertexData &vertexData);
			static void loadPoses(TiXmlHandle hRoot, IScene *scn, bool meshSharedGeometry);
			static void loadPoseAnimations(TiXmlHandle hRoot, ScenePoses *scn);
			static void loadBoneAssignements(TiXmlElement *pElem, curitiba::geometry::MeshBones *mb);
			static void loadSkeleton(TiXmlHandle hRoot, SceneSkeleton *sk) throw (std::string);
			static void loadSkeletonElements(TiXmlHandle hRoot, SceneSkeleton *sk)  throw (std::string);

			enum {
				TRIANGLE_LIST,
				TRIANGLE_STRIP,
				TRIANGLE_FAN
			};
		};
	};
};

#endif //OGREMESHLOADER_H
