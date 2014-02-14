#include <curitiba/config.h>
#include <curitiba/loader/assimploader.h>
#include <curitiba/slogger.h>

#include <curitiba.h>
#include <curitiba/render/irenderable.h>
#include <curitiba/geometry/mesh.h>
#include <curitiba/material/materialgroup.h>
#include <curitiba/material/material.h>
#include <curitiba/render/vertexdata.h>
#include <curitiba/system/fileutil.h>

#include <fstream>

using namespace curitiba::loader;
using namespace curitiba::geometry;

Assimp::Importer AssimpLoader::importer;

void
AssimpLoader::loadScene(curitiba::scene::IScene *aScene, std::string &aFilename, std::string &params)
{

	std::string path = FileUtil::GetPath(aFilename);
	const aiScene *sc;
	//check if file exists
	std::ifstream fin(aFilename.c_str());
	if(!fin.fail()) {
		fin.close();
	}

	else{
		SLOG ("Cannot open file: %s", aFilename.c_str());
		return;
	}

	sc = importer.ReadFile( aFilename, aiProcess_CalcTangentSpace|aiProcess_Triangulate);//aiProcessPreset_TargetRealtime_Quality);

	// If the import failed, report it
	if( !sc)
	{
		SLOG ("Error opening file: %s\n%s\n", aFilename.c_str(),importer.GetErrorString());
		return;
	}

	unsigned int order;
	if (params == "")
		order = XYZ;
	else if (params == "SWAP_YZ")
		order = XZ_Y;

	std::map<unsigned int, std::string> meshNameMap;

		// For each mesh
	for (unsigned int n = 0; n < sc->mNumMeshes; ++n)
	{
		const  aiMesh* mesh = sc->mMeshes[n];

		if (mesh->mPrimitiveTypes != 4)
			continue;

		Mesh *renderable =  (Mesh *)RESOURCEMANAGER->createRenderable("Mesh", mesh->mName.data, aFilename);
		meshNameMap[n] = renderable->getName();
		renderable->setDrawingPrimitive(curitiba::render::IRenderer::TRIANGLES);

		std::vector<unsigned int> *indices = new std::vector<unsigned int>;

		for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
			const  aiFace* face = &mesh->mFaces[t];
			indices->push_back(face->mIndices[0]);
			indices->push_back(face->mIndices[1]);
			indices->push_back(face->mIndices[2]);
		}
		MaterialGroup *aMaterialGroup = new MaterialGroup;
		aMaterialGroup->setIndexList(indices);
		aMaterialGroup->setParent(renderable);

		 aiMaterial *mtl = sc->mMaterials[mesh->mMaterialIndex];
		aiString name;
		mtl->Get(AI_MATKEY_NAME,name);
		aMaterialGroup->setMaterialName(name.data);

		renderable->addMaterialGroup(aMaterialGroup);

		
		VertexData &vertexData = renderable->getVertexData();


		if (mesh->HasPositions()) {
			std::vector<curitiba::math::vec4>* vertex = readGL3FArray((float *)mesh->mVertices,mesh->mNumVertices, order, 1.0f);
			vertexData.setDataFor(VertexData::getAttribIndex("position"), vertex); 
		}

		if (mesh->HasNormals()) {
			std::vector<curitiba::math::vec4>* normal = readGL3FArray((float *)mesh->mNormals,mesh->mNumVertices, order, 0.0f);
			vertexData.setDataFor(VertexData::getAttribIndex("normal"), normal);  
		}

		// buffer for vertex texture coordinates
		if (mesh->HasTextureCoords(0)) {
			std::vector<curitiba::math::vec4>* texCoord = new std::vector<curitiba::math::vec4>(mesh->mNumVertices);
			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

				texCoord->at(k).x   = mesh->mTextureCoords[0][k].x;
				texCoord->at(k).y   = mesh->mTextureCoords[0][k].y; 
				texCoord->at(k).z  = mesh->mTextureCoords[0][k].z; 
				//texCoord->at(k).z = 0.0;
				texCoord->at(k).w = 1.0;
			}
			vertexData.setDataFor(VertexData::getAttribIndex("texCoord0"), texCoord); 
		}

		if (!MATERIALLIBMANAGER->hasMaterial (DEFAULTMATERIALLIBNAME, name.data)) {

			//Material *m = new Material();
			//m->setName(name.data);
			Material *m = MATERIALLIBMANAGER->createMaterial(name.data);
			aiString texPath;	
			if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)){
				m->createTexture(0,FileUtil::GetFullPath(path,texPath.data));
			}
		
			float c[4];
			aiColor4D color;

			if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &color)) {
				color4_to_float4(&color, c);
				m->getColor().setColorComponent(curitiba::material::ColorMaterial::DIFFUSE, c);
			}

			if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &color)) {
				color4_to_float4(&color, c);
				m->getColor().setColorComponent(curitiba::material::ColorMaterial::AMBIENT, c);
			}

			if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &color)) {
				color4_to_float4(&color, c);
				m->getColor().setColorComponent(curitiba::material::ColorMaterial::SPECULAR, c);
			}

			if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &color)) {
				color4_to_float4(&color, c);
				m->getColor().setColorComponent(curitiba::material::ColorMaterial::EMISSION, c);
			}
			float shininess = 0.0;
			unsigned int max;
			if (AI_SUCCESS == aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max))
				m->getColor().setColorComponent(curitiba::material::ColorMaterial::SHININESS, c);

			//MATERIALLIBMANAGER->addMaterial(DEFAULTMATERIALLIBNAME, m);

		}
	}
	SimpleTransform m;
	m.setIdentity();
	recursiveWalk(aScene, aFilename, sc, sc->mRootNode, m, meshNameMap);

}



void
AssimpLoader::writeScene(curitiba::scene::IScene *aScene, std::string &aFilename)
{


}


void 
AssimpLoader::recursiveWalk (curitiba::scene::IScene *aScene, std::string &aFilename,
									const  aiScene *sc, const  aiNode* nd, SimpleTransform &m,
									std::map<unsigned int, std::string> meshNameMap)
{
	ITransform *original = m.clone();
	
	 aiMatrix4x4 mA = nd->mTransformation;
	mA.Transpose();
	float f[16];
	memcpy(f,&mA, sizeof(float)*16);


	SimpleTransform aux;
	mat4 m4;
	m4.setMatrix(f);
	aux.setMat44(m4);
	m.compose(aux);

	for (unsigned int n=0; n < nd->mNumMeshes; ++n) {
	
		if (sc->mMeshes[nd->mMeshes[n]]->mPrimitiveTypes == 4) {
		//sc->mMeshes[nd->mMeshes[n]]->mName.data;
		SceneObject *so = SceneObjectFactory::create("SimpleObject");
		so->setRenderable(RESOURCEMANAGER->getRenderable(meshNameMap[nd->mMeshes[n]],""));
		so->setTransform(m.clone());
		aScene->add(so);
		}
	}

	for (unsigned int n=0; n < nd->mNumChildren; ++n){
		recursiveWalk(aScene, aFilename, sc, nd->mChildren[n], m, meshNameMap);
	}

	m.clone(original);
	delete original;

}


std::vector<curitiba::math::vec4>* 
AssimpLoader::readGL3FArray(float* a, unsigned int arraysize, unsigned int order, float w)
{
	std::vector<curitiba::math::vec4> *v = new std::vector<curitiba::math::vec4>(arraysize);

	for(unsigned int i=0;i<arraysize;i++)
		if (order == XYZ)
			(*v)[i] = curitiba::math::vec4((float)a[(i)*3], (float)a[((i)*3)+1], (float)a[((i)*3)+2], w);
		else 
			(*v)[i] = curitiba::math::vec4((float)a[(i)*3], -(float)a[((i)*3)+2], (float)a[((i)*3)+1], w);

	return v;
}

std::vector<curitiba::math::vec4>* 
AssimpLoader::readGL2FArray(float* a, unsigned int arraysize)
{
	std::vector<vec4> *v = new std::vector<curitiba::math::vec4>(arraysize);

	for(unsigned int i=0;i<arraysize;i++)
		(*v)[i] = curitiba::math::vec4((float)a[(i)*2], (float)a[((i)*2)+1], 0.0f, 1.0f);

	return v;
}


void 
AssimpLoader::color4_to_float4(const  aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}


void 
AssimpLoader::set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}
