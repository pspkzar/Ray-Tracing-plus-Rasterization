#pragma warning( disable: 4290)


#include <curitiba/loader/ogremeshloader.h>

#include <curitiba/scene/scenefactory.h>
#include <curitiba/scene/sceneobjectfactory.h>

#include <curitiba/render/irenderable.h>
#include <curitiba/geometry/poseoffset.h>
#include <curitiba/geometry/meshwithpose.h>

#include <curitiba/system/textutil.h>

#include <curitiba/config.h>
#include <curitiba/slogger.h>
#include <curitiba/errors.h>


#ifdef CURITIBA_PLATFORM_WIN32
#include <curitiba/system/dirent.h>
#else
#include <dirent.h>
#include <sys/types.h>
#endif

using namespace curitiba::loader;
using namespace curitiba::math;
using namespace curitiba::material;
using namespace curitiba::render;
using namespace curitiba::scene;
using namespace curitiba::geometry;
//
using namespace curitiba::event_;
//

std::string OgreMeshLoader::m_Path = "";
std::string OgreMeshLoader::m_MeshFile = "";
std::string OgreMeshLoader::m_SkeletonFile = "";

/*--------------------------------------------------------------------
Project Specification

<?xml version="1.0" ?>
<project name="teste1-shadows">
	<assets>
		...
	</assets>
	<pipelines>
		...
	</pipelines>
</project>

-------------------------------------------------------------------*/

void
OgreMeshLoader::loadScene (IScene* scn, std::string file) throw (std::string)
{
	OgreMeshLoader::m_Path = FileUtil::GetPath(file);
	OgreMeshLoader::m_MeshFile = file;

	TiXmlDocument doc (file.c_str());
	bool loadOkay = doc.LoadFile();

	if (!loadOkay) 
		throw("Parsing Error in file: " + file);

	std::vector<std::string> matLibs;

	std::string meshType;
	bool meshSharedGeometry;

	TiXmlHandle hDoc (&doc);
	TiXmlHandle hRoot (0);
	TiXmlElement *pElem;

	{ //root
		pElem = hDoc.FirstChildElement().Element();
		if (0 == pElem) {
			throw("Parsing Error in file: " + file);
		}

		hRoot = TiXmlHandle (pElem);
	}

	if (hRoot.FirstChildElement("poses").Element())
		meshType = "MeshPose";
	else if (hRoot.FirstChildElement("skeleton").Element() || 
			 hRoot.FirstChildElement("skeletonlink").Element())
		meshType = "MeshBones";
	else
		meshType = "Mesh";

//	scn->setName(file);

	try {
		IRenderable *m = loadSharedGeometry(hRoot, scn, meshType);	

		if (m) 
			meshSharedGeometry = true;
		else 
			meshSharedGeometry = false;
		
		loadSubMeshes(hRoot, scn, m, meshType); 

		loadSubMeshNames(hRoot,scn, meshSharedGeometry);

		if (scn->getType() == "ScenePoses") {
			loadPoses(hRoot,scn, meshSharedGeometry);
			loadPoseAnimations(hRoot,(ScenePoses *)scn);
		}
		else if (scn->getType() == "SceneSkeleton") {
			loadSkeleton(hRoot, (SceneSkeleton *)scn);
		}
	}
	catch(std::string &s) {
		throw(s);
	}

}

void
OgreMeshLoader::loadSkeleton(TiXmlHandle hRoot, SceneSkeleton *sk)  throw (std::string)
{
	TiXmlElement *pElem;

	pElem = hRoot.FirstChildElement("skeletonlink").Element();

	if (pElem) {
	
		const char *pName = pElem->Attribute("name");

		if (!pName)
			CURITIBA_THROW("Skeleton Link misses file name. File: %s", pName, OgreMeshLoader::m_MeshFile.c_str());

		char name[512];
		sprintf(name,"%s.xml", pName);

		OgreMeshLoader::m_SkeletonFile = FileUtil::GetFullPath(m_Path, name);
		TiXmlDocument doc (OgreMeshLoader::m_SkeletonFile.c_str());
		bool loadOkay = doc.LoadFile();

		if (!true == loadOkay) 
			CURITIBA_THROW ("Parsing Error: %s", OgreMeshLoader::m_SkeletonFile.c_str());

		TiXmlHandle hDoc (&doc);
		TiXmlHandle hRoot (0);
		{ //root
			pElem = hDoc.FirstChildElement().Element();
			if (0 == pElem) {
				CURITIBA_THROW ("Parsing Error: %s", OgreMeshLoader::m_SkeletonFile.c_str() );
			}

			hRoot = TiXmlHandle (pElem);
		}

		try{
			loadSkeletonElements(hRoot,sk);
		}
		catch(std::string &s) {
			throw(s);
		}
	
	}
	else
		loadSkeletonElements(hRoot, sk);
}

void
OgreMeshLoader::loadSkeletonElements(TiXmlHandle hRoot, SceneSkeleton *sk)  throw (std::string)
{
	// load bones

	vec3 pos, axis;
	float angle;
	int id;

	TiXmlElement *pElem = hRoot.FirstChild("bones").FirstChild("bone").Element();
	for ( ; pElem != 0; pElem = pElem->NextSiblingElement("bone")) {

		const char *pName = pElem->Attribute("name");
		const char *pId = pElem->Attribute("id");

		if (!pName || !pId)
			throw("Bone has no name or id. File: %s", OgreMeshLoader::m_SkeletonFile.c_str());

		id = curitiba::system::textutil::ParseInt(pId);

		TiXmlElement *pPosition = pElem->FirstChildElement("position");

		if (!pPosition)
			CURITIBA_THROW("Bone %s has no position. File: %s", pName, OgreMeshLoader::m_SkeletonFile.c_str());
		
		const char* pX = pPosition->Attribute("x"); 
		const char* pY = pPosition->Attribute("y"); 
		const char* pZ = pPosition->Attribute("z"); 

		if (!pX || !pY || !pZ)
			CURITIBA_THROW("Bone %s has position x, y, or z missing. File: %s", pName, OgreMeshLoader::m_SkeletonFile.c_str());

		pos.set(curitiba::system::textutil::ParseFloat(pX),
			    curitiba::system::textutil::ParseFloat(pY),
				curitiba::system::textutil::ParseFloat(pZ));

		TiXmlElement *pRotation = pElem->FirstChildElement("rotation");

		if (!pRotation)
			CURITIBA_THROW("Bone %s has position x, y, or z missing. File: %s", pName, OgreMeshLoader::m_SkeletonFile.c_str());

		const char* pAngle = pRotation->Attribute("angle");

		if (!pAngle)
			CURITIBA_THROW("Bone %s has no angle in field rotation. File: %s", pName, OgreMeshLoader::m_SkeletonFile.c_str());

		angle = curitiba::system::textutil::ParseFloat(pAngle);

		TiXmlElement *pAxis = pRotation->FirstChildElement("axis");

		if (!pAxis)
			CURITIBA_THROW("Bone %s has no axis in field rotation. File: %s", pName, OgreMeshLoader::m_SkeletonFile.c_str());

		const char *pAx = pAxis->Attribute("x");
		const char *pAy = pAxis->Attribute("y");
		const char *pAz = pAxis->Attribute("z");

		if (!pAx || !pAy || !pAz)
			CURITIBA_THROW("Bone %s has rotation axis x, y, or z missing. File: %s", pName, OgreMeshLoader::m_SkeletonFile.c_str());

		axis.set(curitiba::system::textutil::ParseFloat(pAx),
			     curitiba::system::textutil::ParseFloat(pAy),
				 curitiba::system::textutil::ParseFloat(pAz));

		sk->addBone(pName, id, pos, angle, axis);
	}

	// load hierarchy

	pElem = hRoot.FirstChild("bonehierarchy").FirstChild("boneparent").Element();
	for ( ; pElem != 0; pElem = pElem->NextSiblingElement("boneparent")) {

		const char *pBone = pElem->Attribute("bone");
		const char *pParent = pElem->Attribute("parent");

		if (!pBone || !pParent)
			CURITIBA_THROW("Bone hierarchy, bone or parent name missing. File: %s", OgreMeshLoader::m_SkeletonFile.c_str());

		sk->setBoneRelation(pBone, pParent);

	}

	// load animations

	float time;

	pElem = hRoot.FirstChild("animations").FirstChild("animation").Element();
	for ( ; pElem != 0; pElem = pElem->NextSiblingElement("animation")) {

		const char *pName = pElem->Attribute("name");
		const char *pLength = pElem->Attribute("length");

		if (!pName || ! pLength) 
		/*{
			char m[512];
			sprintf(m," Name or length missing in bone animation. File: ");
			throw (std::string("bla"));
		}*/
			CURITIBA_THROW("Name or length missing in bone animation. File: %s", OgreMeshLoader::m_SkeletonFile.c_str());

		sk->addAnim(pName, curitiba::system::textutil::ParseFloat(pLength));

		// loading tracks
		TiXmlElement *pElemTrack = pElem->FirstChildElement("tracks")->FirstChildElement("track");
		for ( ; pElemTrack != 0; pElemTrack = pElemTrack->NextSiblingElement("track")) {

			const char *pBone = pElemTrack->Attribute("bone");

			// loading keyframes
			TiXmlElement *pElemKeyFrame = pElemTrack->FirstChildElement("keyframes")->FirstChildElement("keyframe");
			for ( ; pElemKeyFrame != 0; pElemKeyFrame = pElemKeyFrame->NextSiblingElement("keyframe")) {

				const char *pTime = pElemKeyFrame->Attribute("time");

				if (!pTime)
					time = 0;
				else
					time = curitiba::system::textutil::ParseFloat(pTime);

				TiXmlElement *pElemTrans = pElemKeyFrame->FirstChildElement("translate");

				if (!pElemTrans)
					CURITIBA_THROW("Missing translate in anim %s for track %s. File: %s", pName, pBone, OgreMeshLoader::m_SkeletonFile.c_str());

				const char *pX = pElemTrans->Attribute("x");
				const char *pY = pElemTrans->Attribute("y");
				const char *pZ = pElemTrans->Attribute("z");

				if (!pX || !pY || !pZ)
					CURITIBA_THROW("Missing coordinates in position. Anim %s for track %s. File: %s", pName, pBone, OgreMeshLoader::m_SkeletonFile.c_str());

				pos.set(curitiba::system::textutil::ParseFloat(pX),
					curitiba::system::textutil::ParseFloat(pY),
					curitiba::system::textutil::ParseFloat(pZ));

				TiXmlElement *pElemRot = pElemKeyFrame->FirstChildElement("rotate");

				if (!pElemRot)
					CURITIBA_THROW("Missing rotate in anim %s for track %s. File: %s", pName, pBone, OgreMeshLoader::m_SkeletonFile.c_str());

				const char *pAngle = pElemRot->Attribute("angle");

				if (!pAngle)
					CURITIBA_THROW("Missing angle in anim %s for track %s. File: %s", pName, pBone, OgreMeshLoader::m_SkeletonFile.c_str());

				angle = curitiba::system::textutil::ParseFloat(pAngle);

				TiXmlElement *pElemAxis = pElemRot->FirstChildElement("axis");

				if (!pElemAxis)
					CURITIBA_THROW("Missing axis in anim %s for track %s. File: %s", pName, pBone, OgreMeshLoader::m_SkeletonFile.c_str());

				const char *pAx = pElemAxis->Attribute("x");
				const char *pAy = pElemAxis->Attribute("y");
				const char *pAz = pElemAxis->Attribute("z");

				if (!pAx || !pAy || !pAz)
					CURITIBA_THROW("Missing coordinates in aixs. Anim %s for track %s. File: %s", pName, pBone, OgreMeshLoader::m_SkeletonFile.c_str());

				axis.set(curitiba::system::textutil::ParseFloat(pAx),
					curitiba::system::textutil::ParseFloat(pAy),
					curitiba::system::textutil::ParseFloat(pAz));

				sk->getAnim(pName).addKeyFrame(sk->seekBoneID(pBone),time, pos, axis, angle);
			}
		}
	}
}

void 
OgreMeshLoader::loadPoseAnimations(TiXmlHandle hRoot, ScenePoses *scn)
{
	float length, time, influence;
	int meshIndex,poseIndex;

	// Load Animations
	TiXmlElement *pElemAnim = hRoot.FirstChild("animations").FirstChild("animation").Element();
	for ( ; pElemAnim != 0; pElemAnim = pElemAnim->NextSiblingElement("animation")) {

		const char *pName = pElemAnim->Attribute("name");
		const char *pLength = pElemAnim->Attribute("length");

		if (!pName || !pLength)
			CURITIBA_THROW("Animation misses name or length. File: %s", OgreMeshLoader::m_MeshFile.c_str());

		length = curitiba::system::textutil::ParseFloat(pLength);

		scn->addAnim(pName, length);

		// Load Tracks
		TiXmlElement *pElemTrack = pElemAnim->FirstChildElement("tracks")->FirstChildElement("track");
		for ( ; pElemTrack != 0; pElemTrack = pElemTrack->NextSiblingElement("track")) {

			const char *pMeshIndex = pElemTrack->Attribute("index");

			if (!pMeshIndex)
				CURITIBA_THROW("Animation Track misses an index. File: %s", OgreMeshLoader::m_MeshFile.c_str());

			meshIndex = curitiba::system::textutil::ParseInt(pMeshIndex);

			//Load keyframes

			TiXmlElement *pElemKF = pElemTrack->FirstChildElement("keyframes")->FirstChildElement("keyframe");
			for ( ; pElemKF != 0; pElemKF = pElemKF->NextSiblingElement("keyframe")) {

				const char *pTime = pElemKF->Attribute("time");

				if (!pTime)
					CURITIBA_THROW("Keyframe in animation %s misses a time. File: %s", pName, OgreMeshLoader::m_MeshFile.c_str());

				time = curitiba::system::textutil::ParseFloat(pTime);

				// Load PoseRefs

				TiXmlElement *pElemPoseRef = pElemKF->FirstChildElement("poseref");
				for ( ; pElemPoseRef != 0; pElemPoseRef = pElemPoseRef->NextSiblingElement("poseref")) {

					const char *pPoseIndex = pElemPoseRef->Attribute("poseindex");
					const char *pInfluence = pElemPoseRef->Attribute("influence");

					if (!pPoseIndex || !pInfluence)
						CURITIBA_THROW("Invalid poseref element in animation %s. File: %s", pName, OgreMeshLoader::m_MeshFile.c_str());

					poseIndex = curitiba::system::textutil::ParseFloat(pPoseIndex);
					influence = curitiba::system::textutil::ParseFloat(pInfluence);

					scn->getAnim(pName).addInfluence(meshIndex,poseIndex,time,influence);
				}
			}
		}
	}
}



void
OgreMeshLoader::loadBoneAssignements(TiXmlElement *pElem, MeshBones *mb)
{
	int vertexIndex,boneIndex;
	float weight;

	TiXmlElement *pElemPose = pElem->FirstChild("boneassignments")->FirstChildElement("vertexboneassignment");
	for ( ; pElemPose != 0; pElemPose = pElemPose->NextSiblingElement("vertexboneassignment")) {
		
		const char *pVertexIndex = pElemPose->Attribute("vertexindex");
		const char *pBoneIndex = pElemPose->Attribute("boneindex");
		const char *pWeight = pElemPose->Attribute("weight");

		if (!pVertexIndex || !pBoneIndex || !pWeight) 
			CURITIBA_THROW("Invalid Vertex Bone Assignment. File: %s", OgreMeshLoader::m_MeshFile.c_str());

		vertexIndex = curitiba::system::textutil::ParseInt(pVertexIndex);
		boneIndex = curitiba::system::textutil::ParseInt(pBoneIndex);
		weight = curitiba::system::textutil::ParseFloat(pWeight);

		mb->addBoneWeight(vertexIndex, boneIndex, weight);
	}	
}


void 
OgreMeshLoader::loadPoses(TiXmlHandle hRoot, IScene *scn, bool meshSharedGeometry)
{
	int index,poseIndex;
	float x,y,z;
	MeshPose *mp;
	PoseOffset *pos;

	TiXmlElement *pElemPose = hRoot.FirstChild("poses").FirstChild("pose").Element();;
	for ( ; pElemPose != 0; pElemPose = pElemPose->NextSiblingElement("pose")) {
		
		const char *pTarget = pElemPose->Attribute("target");
		const char *pIndex = pElemPose->Attribute("index");
		const char *pName = pElemPose->Attribute("name");

		if (!pTarget || !pName || !pIndex) 
			CURITIBA_THROW("Pose misses target, index or name. File: %s", OgreMeshLoader::m_MeshFile.c_str());

		if (strcmp(pTarget,"submesh") == 0) 
			index = curitiba::system::textutil::ParseInt(pIndex);
		else 
			index = 0;

		pos = new PoseOffset(scn->getSceneObject(index)->getRenderable().getNumberOfVertices());

		TiXmlElement *pElemPoseOffset = pElemPose->FirstChildElement("poseoffset");
		for ( ; pElemPoseOffset != 0; pElemPoseOffset = pElemPoseOffset->NextSiblingElement("poseoffset")) {

			const char *pPoseIndex = pElemPoseOffset->Attribute("index");
			const char *pX = pElemPoseOffset->Attribute("x");
			const char *pY = pElemPoseOffset->Attribute("y");
			const char *pZ = pElemPoseOffset->Attribute("z");

			if (!pX || !pY || !pZ || !pPoseIndex)
				CURITIBA_THROW("Invalid PoseOffset. File: %s", OgreMeshLoader::m_MeshFile.c_str());

			x = curitiba::system::textutil::ParseFloat(pX);
			y = curitiba::system::textutil::ParseFloat(pY);
			z = curitiba::system::textutil::ParseFloat(pZ);
			poseIndex = curitiba::system::textutil::ParseInt(pPoseIndex);

			pos->addPoseOffset(poseIndex,x,y,z);

		}
		mp = (MeshPose *)scn->getSceneObject(index)->_getRenderablePtr();
		mp->addPose(pName,pos);	
	}	
}

void 
OgreMeshLoader::loadSubMeshNames(TiXmlHandle hRoot, IScene *scn, bool meshSharedGeometry)
{
	TiXmlElement *pElemSubMeshName = hRoot.FirstChild("submeshnames").FirstChild("submeshname").Element();;
	for ( ; pElemSubMeshName != 0; pElemSubMeshName = pElemSubMeshName->NextSiblingElement("submeshname")) {
		
		const char *pName = pElemSubMeshName->Attribute("name");
		const char *pIndex = pElemSubMeshName->Attribute("index");

		if (!pName || !pIndex) 
			CURITIBA_THROW("Invalid submesh name definition. File: %s", OgreMeshLoader::m_MeshFile.c_str());

		int index = curitiba::system::textutil::ParseInt(pIndex);

		if (!meshSharedGeometry)
			scn->getSceneObject(index)->setName(pName);
	}
}


void 
OgreMeshLoader::loadVertexElement(TiXmlElement *pElemVertexAttrib, vec4 *vertexElem) 
{
	float fx,fy,fz;

	const char *x = pElemVertexAttrib->Attribute("x");
	const char *y = pElemVertexAttrib->Attribute("y");
	const char *z = pElemVertexAttrib->Attribute("z");
	
	if (!x || !y || !z)
		CURITIBA_THROW("Invalid vertex element, missing x, y or z coordinates. File: %s", OgreMeshLoader::m_MeshFile.c_str());

	fx = curitiba::system::textutil::ParseFloat(x);
	fy = curitiba::system::textutil::ParseFloat(y);
	fz = curitiba::system::textutil::ParseFloat(z);

	// Add value to array
	vertexElem->set(fx,fy,fz);
}

void 
OgreMeshLoader::loadTextureCoordElement(TiXmlElement *pElemVertexAttrib, vec4 *vertexElem) 
{
	float fx,fy,fz;

	const char *x = pElemVertexAttrib->Attribute("u");
	const char *y = pElemVertexAttrib->Attribute("v");
	const char *z = pElemVertexAttrib->Attribute("w");
	
	if (!x || !y )
		CURITIBA_THROW("Invalid texture coordinate, missing u or v coordinates. File: %s", OgreMeshLoader::m_MeshFile.c_str());

	fx = curitiba::system::textutil::ParseFloat(x);
	fy = curitiba::system::textutil::ParseFloat(y);
	if (z)
		fz = curitiba::system::textutil::ParseFloat(z);
	else 
		fz = 0.0f;

	// Add value to array
	vertexElem->set(fx,fy,fz);
}

void
OgreMeshLoader::loadVertexBuffer(TiXmlElement *pElemVertexBuffer, VertexData &vertexData)
{
	std::vector<VertexData::Attr> *vertices, *normals, *tangents, *binormals, *texCoord0, *texCoord1,
		*texCoord2, *texCoord3, *texCoord4, *texCoord5, *texCoord6, *texCoord7; 

	// count actual vertices
	int actualVertices = 0;
	bool loadVertices = false, loadNormals = false, loadTangents = false,
		loadBinormals = false;
	TiXmlElement *pElemVertex = pElemVertexBuffer->FirstChildElement("vertex");
	for ( ; pElemVertex != 0; actualVertices++, pElemVertex = pElemVertex->NextSiblingElement("vertex")) {
	}

	const char *pPositions	= pElemVertexBuffer->Attribute("positions");
	const char *pNormals	= pElemVertexBuffer->Attribute("normals");
	const char *pTangents = pElemVertexBuffer->Attribute("tangents");
	const char *pBinormals = pElemVertexBuffer->Attribute("binormals");
	const char *pTextureCoords = pElemVertexBuffer->Attribute("texture_coords");
	int iTextureCoords;
	if (pTextureCoords)
		iTextureCoords = textutil::ParseInt(pTextureCoords);
	else
		iTextureCoords = 0;

	// Alocate Space
	if (pPositions && strcmp(pPositions,"true") == 0) {
		vertices = new std::vector<VertexData::Attr>(actualVertices);
		vertexData.setDataFor (VertexData::getAttribIndex("position"), vertices);
		loadVertices = true;
	}
	if (pNormals && strcmp(pNormals,"true") == 0) {
		normals = new std::vector<VertexData::Attr>(actualVertices);
		vertexData.setDataFor (VertexData::getAttribIndex("normal"), normals);
		loadNormals = true;
	}
	if (pTangents && strcmp(pTangents,"true") == 0) {
		tangents = new std::vector<VertexData::Attr>(actualVertices);
		vertexData.setDataFor (VertexData::getAttribIndex("tangent"), tangents);
		loadTangents = true;
	}
	if (pBinormals && strcmp(pBinormals,"true") == 0) {
		binormals = new std::vector<VertexData::Attr>(actualVertices);
		vertexData.setDataFor (VertexData::getAttribIndex("binormal"), binormals);
		loadBinormals = true;
	}
	if (iTextureCoords >0) {
		texCoord0 = new std::vector<VertexData::Attr>(actualVertices);
		vertexData.setDataFor (VertexData::getAttribIndex("texCoord0"), texCoord0);
	}
	if (iTextureCoords >1){
		texCoord1 = new std::vector<VertexData::Attr>(actualVertices);
		vertexData.setDataFor (VertexData::getAttribIndex("texCoord1"), texCoord1);
	}
	if (iTextureCoords >2) {
		texCoord2 = new std::vector<VertexData::Attr>(actualVertices);
		vertexData.setDataFor (VertexData::getAttribIndex("texCoord2"), texCoord2);
	}
	if (iTextureCoords >3) {
		texCoord3 = new std::vector<VertexData::Attr>(actualVertices);
		vertexData.setDataFor (VertexData::getAttribIndex("texCoord3"), texCoord3);
	}
	if (iTextureCoords >4) {
		texCoord4 = new std::vector<VertexData::Attr>(actualVertices);
		vertexData.setDataFor (VertexData::getAttribIndex("texCoord4"), texCoord4);
	}
	if (iTextureCoords >5) {
		texCoord5 = new std::vector<VertexData::Attr>(actualVertices);
		vertexData.setDataFor (VertexData::getAttribIndex("texCoord5"), texCoord5);
	}
	if (iTextureCoords >6) {
		texCoord6 = new std::vector<VertexData::Attr>(actualVertices);
		vertexData.setDataFor (VertexData::getAttribIndex("texCoord6"), texCoord6);
	}
	if (iTextureCoords >7) {
		texCoord7 = new std::vector<VertexData::Attr>(actualVertices);
		vertexData.setDataFor (VertexData::getAttribIndex("texCoord7"), texCoord7);
	}


	TiXmlElement *pElemVertexAttrib;
	int i = 0;
	pElemVertex = pElemVertexBuffer->FirstChildElement("vertex");
	for ( ; pElemVertex != 0; i++, pElemVertex = pElemVertex->NextSiblingElement("vertex")) {

		pElemVertexAttrib = pElemVertex->FirstChildElement("position");
		if (pElemVertexAttrib) 
			loadVertexElement(pElemVertexAttrib,&vertices->at(i));

		pElemVertexAttrib = pElemVertex->FirstChildElement("normal");
		if (pElemVertexAttrib) 
			loadVertexElement(pElemVertexAttrib,&normals->at(i));

		pElemVertexAttrib = pElemVertex->FirstChildElement("tangent");
		if (pElemVertexAttrib) 
			loadVertexElement(pElemVertexAttrib,&tangents->at(i));

		pElemVertexAttrib = pElemVertex->FirstChildElement("binormal");
		if (pElemVertexAttrib) 
			loadVertexElement(pElemVertexAttrib,&binormals->at(i));

		pElemVertexAttrib = pElemVertex->FirstChildElement("texcoord");
		if (pElemVertexAttrib)
			loadTextureCoordElement(pElemVertexAttrib,&texCoord0->at(i));

		if (iTextureCoords > 1) {
			pElemVertexAttrib = pElemVertexAttrib->NextSiblingElement("texcoord");
			if (pElemVertexAttrib)
				loadTextureCoordElement(pElemVertexAttrib,&texCoord1->at(i));
		}

		if (iTextureCoords > 2) {
			pElemVertexAttrib = pElemVertexAttrib->NextSiblingElement("texcoord");
			if (pElemVertexAttrib)
				loadTextureCoordElement(pElemVertexAttrib,&texCoord2->at(i));
		}

		if (iTextureCoords > 3) {
			pElemVertexAttrib = pElemVertexAttrib->NextSiblingElement("texcoord");
			if (pElemVertexAttrib)
				loadTextureCoordElement(pElemVertexAttrib,&texCoord3->at(i));
		}

		if (iTextureCoords > 4) {
			pElemVertexAttrib = pElemVertexAttrib->NextSiblingElement("texcoord");
			if (pElemVertexAttrib)
				loadTextureCoordElement(pElemVertexAttrib,&texCoord4->at(i));
		}

		if (iTextureCoords > 5) {
			pElemVertexAttrib = pElemVertexAttrib->NextSiblingElement("texcoord");
			if (pElemVertexAttrib)
				loadTextureCoordElement(pElemVertexAttrib,&texCoord5->at(i));
		}

		if (iTextureCoords > 6) {
			pElemVertexAttrib = pElemVertexAttrib->NextSiblingElement("texcoord");
			if (pElemVertexAttrib)
				loadTextureCoordElement(pElemVertexAttrib,&texCoord6->at(i));
		}

		if (iTextureCoords > 7) {
			pElemVertexAttrib = pElemVertexAttrib->NextSiblingElement("texcoord");
			if (pElemVertexAttrib)
				loadTextureCoordElement(pElemVertexAttrib,&texCoord7->at(i));
		}

	}
}


void 
OgreMeshLoader::loadVertexBuffers(TiXmlElement *pElem, VertexData &vertexData) 
{
	// for each vertex buffer
	TiXmlElement *pElemVertexBuffer = pElem->FirstChildElement("vertexbuffer");
	for ( ; pElemVertexBuffer != 0; pElemVertexBuffer = pElemVertexBuffer->NextSiblingElement("vertexbuffer")) {
	
		loadVertexBuffer(pElemVertexBuffer, vertexData);
	}
}


void
OgreMeshLoader::loadGeometry(TiXmlElement *pElem,VertexData &vertexData)
{
	TiXmlElement *pElemGeometry;
	pElemGeometry = pElem->FirstChildElement("geometry");
	const char *vertexcount = pElemGeometry->Attribute("vertexcount");

	if (!vertexcount) 
		CURITIBA_THROW("Missing vertex count field File: %s", OgreMeshLoader::m_MeshFile.c_str());

	int iVertexCount = textutil::ParseInt(vertexcount);
	loadVertexBuffers(pElemGeometry, vertexData);
}


void 
OgreMeshLoader::loadFaces(TiXmlElement *pElem, MaterialGroup *mg, unsigned int operationType)
{
	unsigned int faceIndex = 0;
	TiXmlElement *pElemFaces = pElem->FirstChildElement("faces");
	
	if (!pElemFaces) 
		CURITIBA_THROW("Missing faces' definition. File: %s", OgreMeshLoader::m_MeshFile.c_str());
	
	const char *pNumFaces = pElemFaces->Attribute("count");
	
	if (!pNumFaces)
		CURITIBA_THROW("Missing face count. File: %s", OgreMeshLoader::m_MeshFile.c_str());

	int iCount = textutil::ParseInt(pNumFaces);
	if (operationType == TRIANGLE_LIST)
		iCount *= 3;
	else
		iCount += 2;
	std::vector<unsigned int> *indices = new std::vector<unsigned int>(iCount);
	
	TiXmlElement *pElemFace = pElemFaces->FirstChildElement("face");
	for ( ; pElemFace != 0; pElemFace = pElemFace->NextSiblingElement("face")) {

		const char *v1 = pElemFace->Attribute("v1");
		const char *v2 = pElemFace->Attribute("v2");
		const char *v3 = pElemFace->Attribute("v3");

		if (!v1 || ((!v2 || !v3) && operationType == TRIANGLE_LIST))
			CURITIBA_THROW("Invalid face definition. File: %s", OgreMeshLoader::m_MeshFile.c_str());

		indices->at(faceIndex) = curitiba::system::textutil::ParseInt(v1);
		faceIndex++;

		if (operationType == TRIANGLE_LIST) {

			indices->at(faceIndex) = curitiba::system::textutil::ParseInt(v2);
			faceIndex++;

			indices->at(faceIndex) = curitiba::system::textutil::ParseInt(v3);
			faceIndex++;
		}
		
	}
	mg->setIndexList(indices);
}


void
OgreMeshLoader::loadSubMeshes(TiXmlHandle handle, IScene *scn, IRenderable *m, std::string meshType) 
{
	TiXmlElement *pElem;
	//std::vector<vec3> *vertices, *normals, *tangents, *binormals, *texCoord0, *texCoord1,
	//	*texCoord2, *texCoord3, *texCoord4, *texCoord5, *texCoord6, *texCoord7; 
	char aux[256];
	unsigned int operationType;

	pElem = handle.FirstChild ("submeshes").FirstChild("submesh").Element();
	for (int index = 0 ; 0 != pElem; pElem = pElem->NextSiblingElement(), index++) {

		// for each submesh
		const char *pMaterialName = pElem->Attribute("material");
		const char *pUseSharedVertices = pElem->Attribute("usesharedvertices");
		const char *pOperationType = pElem->Attribute("operationtype");

		if (!pOperationType)
			operationType = TRIANGLE_LIST;

		if (pOperationType) {
			if (!strcmp(pOperationType,"triangle_strip"))
				operationType = TRIANGLE_STRIP;
			else if (!strcmp(pOperationType,"triangle_fan"))
				operationType = TRIANGLE_FAN;
			else
				operationType = TRIANGLE_LIST;
		}
		else
			operationType = TRIANGLE_LIST;
			
		Material *mat;
		if (0 == pMaterialName)  // Material is required
			mat = MATERIALLIBMANAGER->createMaterial("Default Material");					
		else
			mat = MATERIALLIBMANAGER->createMaterial(pMaterialName);
		//MATERIALLIBMANAGER->addMaterial(DEFAULTMATERIALLIBNAME, mat);

		SceneObject *scnObj;
		
		if (!pUseSharedVertices || strcmp("true", pUseSharedVertices) == 0) {
		
			MaterialGroup *mg = new MaterialGroup();
			mg->setMaterialName(pMaterialName);
			mg->setParent(m);
			m->setDrawingPrimitive(operationType);
			loadFaces(pElem, mg, operationType);
			m->addMaterialGroup(mg);

		}
		else {

			IRenderable *m = RESOURCEMANAGER->createRenderable(meshType, aux, OgreMeshLoader::m_MeshFile);
			m->setDrawingPrimitive(operationType);
			scnObj = SceneObjectFactory::create("SimpleObject");
			sprintf(aux,"Unnamed_%d", index);
			scnObj->setName(aux);
			if (meshType != "Mesh")
				scnObj->setStaticCondition(false);
			//m->setName(aux);
			//RESOURCEMANAGER->addRenderable(m, OgreMeshLoader::m_MeshFile);
			scnObj->setRenderable(m);
			MaterialGroup *mg = new MaterialGroup();
			mg->setMaterialName(pMaterialName);
			mg->setParent(m);

			VertexData &vertexData = m->getVertexData();
			loadGeometry(pElem,vertexData);
			

			if (meshType == "MeshPose") {
				MeshPose *mp = (MeshPose *)scnObj->_getRenderablePtr();
				mp->setReferencePose(vertexData.getDataOf(VertexData::getAttribIndex("position")));
			}
			if (meshType == "MeshBones") {
				MeshBones *mb = (MeshBones *)scnObj->_getRenderablePtr();
				loadBoneAssignements(pElem, mb);
			}
			loadFaces(pElem, mg, operationType);
			m->addMaterialGroup(mg);
			
			scn->add(scnObj);
		}
	}
}

IRenderable *
OgreMeshLoader::loadSharedGeometry (TiXmlHandle hRoot, IScene *scn, std::string meshType)
{
	IRenderable *m;
	TiXmlElement *pElem;
//	char aux[256];
	SceneObject *scnObj;

	pElem = hRoot.FirstChild ("sharedgeometry").Element();

	if (pElem) {
		m = RESOURCEMANAGER->createRenderable(meshType, "Unnamed_shared", OgreMeshLoader::m_MeshFile);
		scnObj = SceneObjectFactory::create("SimpleObject");
		//sprintf(aux,"Unnamed_shared", index);
		scnObj->setName("Unnamed_shared");
		if (meshType != "Mesh")
			scnObj->setStaticCondition(false);
		//m->setName();
		//RESOURCEMANAGER->addRenderable(m, );

		scnObj->setRenderable(m);

		VertexData &vertexData = m->getVertexData();

		loadVertexBuffers(pElem,vertexData);
		if (meshType == "MeshPose") {
			MeshPose *mp = (MeshPose *)scnObj->_getRenderablePtr();
			mp->setReferencePose(vertexData.getDataOf(VertexData::getAttribIndex("position")));
		}
		scn->add(scnObj);
	}
	else
		m = NULL;

	return m;
}