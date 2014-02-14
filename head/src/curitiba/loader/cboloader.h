#ifndef CBOLOADER_H
#define CBOLOADER_H

#include <curitiba/scene/iscene.h>
#include <curitiba/render/vertexdata.h>
#include <curitiba/material/material.h>
#include <curitiba/scene/octreeByMatscene.h>

using namespace curitiba::render;
using namespace curitiba::material;
using namespace curitiba::scene;

namespace curitiba 
{

	namespace loader 
	{
		class CBOLoader
		{
		public:	
			static void loadScene (curitiba::scene::IScene *aScene, std::string &aFilename);
			static void writeScene (curitiba::scene::IScene *aScene, std::string &aFilename);

		private:
			CBOLoader(void) {};
			~CBOLoader(void) {};

			static std::string m_FileName;

			static void _writeMaterial(std::string matName, std::string path, std::fstream &f);
			static void _readMaterial(std::string path, std::fstream &f);
			static void _writeVertexData (VertexData& aVertexData, std::fstream &f) ;
			static void _readVertexData (VertexData& aVertexData, std::fstream &f);
			static void _writeIndexData (IndexData& aVertexData, std::fstream &f) ;
			static void _readIndexData (IndexData& aVertexData, std::fstream &f);
			static void _ignoreVertexData (std::fstream &f);
			static void _writeString (const std::string& aString, std::fstream &f);
			static void _readString ( char *buffer, std::fstream &f);
			static void _ignoreString (std::fstream &f);

			static void _writeOctreeByMat(OctreeByMatScene *aScene, std::fstream &f);
			static void _writeOctreeByMatNode(OctreeByMatNode *n, std::fstream &f)	;	
			static void _writeOctreeByMatSceneObject(SceneObject *so, std::fstream &f)	;

			static void _readOctreeByMat(OctreeByMatScene *aScene, std::fstream &f);
			static void _readOctreeByMatNode(OctreeByMatNode *n, std::fstream &f)	;	
			static void _readOctreeByMatSceneObject(SceneObject *so, std::fstream &f)	;
		};
	};
};

#endif //CBOLOADER_H
