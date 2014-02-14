#include <curitiba/loader/projectloader.h>

#include <curitiba.h>

#include <curitiba/material/programvalue.h>
#include <curitiba/render/pipeline.h>
#include <curitiba/render/passfactory.h>

#ifdef CURITIBA_OPTIX
#include <curitiba/render/passOptix.h>
#endif

#include <curitiba/system/textutil.h>
//
#include <curitiba/event/sensorfactory.h>
#include <curitiba/event/interpolatorFactory.h>
#include <curitiba/scene/sceneobjectfactory.h>
#include <curitiba/event/route.h>
#include <curitiba/event/objectAnimation.h>
#include <curitiba/render/rendertarget.h>
#include <curitiba/scene/geometryobject.h>
#include <curitiba/geometry/primitive.h>
#include <curitiba/math/transformfactory.h>

#include <curitiba/slogger.h>

#include <curitiba/config.h>

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

std::string ProjectLoader::m_Path = "";
std::string ProjectLoader::m_File = "";

char ProjectLoader::m_pFullName[256] = "";

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
ProjectLoader::load (std::string file, int *width, int *height, bool *tangents, bool *triangleIDs)
{
	ProjectLoader::m_Path = FileUtil::GetPath(file);
	ProjectLoader::m_File = file;

	TiXmlDocument doc (file.c_str());
	bool loadOkay = doc.LoadFile();
	std::vector<std::string> matLibs;

	if (!loadOkay) {

		CURITIBA_THROW("Parsing Error -%s- Line(%d) Column(%d) in file: %s", doc.ErrorDesc(), doc.ErrorRow(), doc.ErrorCol(),file.c_str());
	}
	TiXmlHandle hDoc (&doc);
	TiXmlHandle hRoot (0);
	TiXmlElement *pElem;


	pElem = hDoc.FirstChildElement().Element();
	if (0 == pElem) {
		CURITIBA_THROW("Parsing Error in file: %s", file.c_str());
	}
	hRoot = TiXmlHandle (pElem);


	try {
		*width = 0;
		*height = 0;
		if (TIXML_SUCCESS == pElem->QueryIntAttribute("width",width) &&
			TIXML_SUCCESS == pElem->QueryIntAttribute("height",height)) {
				if (*width <= 0 || *height <= 0) {
					*width = 0;
					*height = 0;
				}
				CURITIBA->setWindowSize(*width, *height);
		}

		const char *pUseTangents = pElem->Attribute("useTangents");
		if (pUseTangents)
			*tangents = !strcmp(pUseTangents, "yes");
		else
			*tangents = false;

		const char *pUseTriangleIDs = pElem->Attribute("useTriangleIDs");
		if (pUseTriangleIDs)
			*triangleIDs = !strcmp(pUseTriangleIDs, "yes");
		else
			*triangleIDs = false;

		bool core;
		const char *pCoreProfile = pElem->Attribute("core");
		if (pCoreProfile)
			core = !strcmp(pCoreProfile, "yes");
		else
			core = false;
		RENDERER->setCore(core);
		
		loadAssets (hRoot, matLibs);
		loadPipelines (hRoot);
	}
	catch(std::string &s) {
		throw(s);
	}
}

/* ----------------------------------------------------------------
Specification of the scenes:

		<scenes>
			<scene name="MainScene" type="Octree" filename = "aScene.cbo" param="bla">
				<file>..\ntg-bin-3\fonte-finallambert.dae</file>
				<folder>..\ntg-bin-pl3dxiv</folder>
			</scene>
			...
		</scenes>

scenes can have multiple "scene" defined
each scene can have files and folders OR a single file containing a scene.
the path may be relative to the project file or absolute

param is passed to the loader
	3DS loader: NO_SWAP to indicate that ZY axis should NOT be swaped 
	( by default they are swapped)
----------------------------------------------------------------- */


void 
ProjectLoader::loadScenes(TiXmlHandle handle) 
{
	TiXmlElement *pElem;

	pElem = handle.FirstChild ("scenes").FirstChild ("scene").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		const char *pName = pElem->Attribute ("name");
		const char *pType = pElem->Attribute ("type");
		const char *pFilename = pElem->Attribute("filename");
		const char *pParam = pElem->Attribute("param");
			
		std::string s;

		if (0 == pName) {
			CURITIBA_THROW("Scene without a name in file %s", ProjectLoader::m_File.c_str()); 					
		}

		SLOG("Scene : %s", pName);
			
		if (pParam == NULL)
			s = "";
		else
			s = pParam;

		IScene *is;
		if (0 == pType) 
			is = RENDERMANAGER->createScene (pName);
		else {
			is = RENDERMANAGER->createScene(pName, pType);
			if (is == NULL)
				CURITIBA_THROW("Invalid type for scene %s in file %s", pName, ProjectLoader::m_File.c_str()); 	
		}
		

		const char *pTransX = pElem->Attribute("transX");
		const char *pTransY = pElem->Attribute("transY");
		const char *pTransZ = pElem->Attribute("transZ");

		const char *pScaleX = pElem->Attribute("scaleX");
		const char *pScaleY = pElem->Attribute("scaleY");
		const char *pScaleZ = pElem->Attribute("scaleZ");
		const char *pScale = pElem->Attribute("scale");

		ITransform *tis = TransformFactory::create("SimpleTransform");

		if (pTransX && pTransY && pTransZ) {
			tis->translate(curitiba::system::textutil::ParseFloat(pTransX),
				curitiba::system::textutil::ParseFloat(pTransY),
				curitiba::system::textutil::ParseFloat(pTransZ));

		}
		if (pScaleX && pScaleY && pScaleZ) {
			tis->scale(curitiba::system::textutil::ParseFloat(pScaleX),
				curitiba::system::textutil::ParseFloat(pScaleY),
				curitiba::system::textutil::ParseFloat(pScaleZ));
		}	
		if (pScale) {
			float scale = curitiba::system::textutil::ParseFloat(pScale);
			tis->scale(scale);
		}

		is->setTransform(tis);

		// the filename should point to a scene
		if (0 != pFilename) {

			if (!FileUtil::exists(FileUtil::GetFullPath(ProjectLoader::m_Path, pFilename)))
				CURITIBA_THROW("Scene file %s does not exist", pFilename); 			

			try {
				curitiba::Curitiba::getInstance()->loadAsset (FileUtil::GetFullPath(ProjectLoader::m_Path, pFilename), pName, s);
			}
			catch(std::string &s) {
				throw(s);
			}
		}
		else {
			handle = TiXmlHandle (pElem);
			TiXmlElement* pElementAux;

			pElementAux = handle.FirstChild("geometry").Element();
			for ( ; 0 != pElementAux; pElementAux = pElementAux->NextSiblingElement()) {
				const char *pName = pElementAux->Attribute("name");
				const char *pPrimType = pElementAux->Attribute ("type");
				const char *pMaterial = pElementAux->Attribute("material");

				pTransX = pElementAux->Attribute("transX");
				pTransY = pElementAux->Attribute("transY");
				pTransZ = pElementAux->Attribute("transZ");

				pScaleX = pElementAux->Attribute("scaleX");
				pScaleY = pElementAux->Attribute("scaleY");
				pScaleZ = pElementAux->Attribute("scaleZ");
				pScale = pElementAux->Attribute("scale");

				if (pPrimType == NULL)
					CURITIBA_THROW("Scene %s has no type", pName); 			

				GeometricObject *go = (GeometricObject *)curitiba::scene::SceneObjectFactory::create("Geometry");
				
				if (go == NULL)
					CURITIBA_THROW("Scene %s has invalid type type", pName); 
				if (pName)
					go->setName(pName);

				Primitive *p = (Primitive *)RESOURCEMANAGER->createRenderable(pPrimType, pName);
				std::string n = p->getParamfName(0);
				unsigned int i = 0;
				while (Primitive::NoParam != n) {

					float value;
					if (TIXML_SUCCESS == pElementAux->QueryFloatAttribute (n.c_str(), &value)) 
						p->setParam(i,value);
					++i;
					n = p->getParamfName(i);
				}
				if (i)
					p->build();
								
				ITransform *t = TransformFactory::create("SimpleTransform");

				if (pTransX && pTransY && pTransZ) {
					t->translate(curitiba::system::textutil::ParseFloat(pTransX),
						curitiba::system::textutil::ParseFloat(pTransY),
						curitiba::system::textutil::ParseFloat(pTransZ));

				}
				if (pScaleX && pScaleY && pScaleZ) {
					t->scale(curitiba::system::textutil::ParseFloat(pScaleX),
						curitiba::system::textutil::ParseFloat(pScaleY),
						curitiba::system::textutil::ParseFloat(pScaleZ));
				}	
				if (pScale) {
					float scale = curitiba::system::textutil::ParseFloat(pScale);
					t->scale(scale);
				}
				go->setTransform(t);
				go->setRenderable(p);

				if (pMaterial) {
					if (!MATERIALLIBMANAGER->hasMaterial(DEFAULTMATERIALLIBNAME,pMaterial)) {
						Material *mat = MATERIALLIBMANAGER->createMaterial(pMaterial);
						//Material *mat = new Material();
						//mat->setName(pMaterial);
						//MATERIALLIBMANAGER->addMaterial(DEFAULTMATERIALLIBNAME, mat);
					}
					go->setMaterial(pMaterial);
				}
				is ->add(go);

			}


			pElementAux = handle.FirstChild ("file").Element();
			for ( ; 0 != pElementAux; pElementAux = pElementAux->NextSiblingElement()) {
				const char * pFileName = pElementAux->GetText();

				if (!FileUtil::exists(FileUtil::GetFullPath(ProjectLoader::m_Path, pFileName)))
					CURITIBA_THROW("Scene file %s does not exist", pFileName); 			

				curitiba::Curitiba::getInstance()->loadAsset (FileUtil::GetFullPath(ProjectLoader::m_Path, pFileName), pName, s);
			}

			pElementAux = handle.FirstChild ("folder").Element();
			for ( ; 0 != pElementAux; pElementAux = pElementAux->NextSiblingElement()) {
				
				DIR *dir;

				struct dirent *ent;

				const char * pDirName = pElementAux->GetText();

				dir = opendir (FileUtil::GetFullPath(ProjectLoader::m_Path, pDirName).c_str());

				if (!dir)
					CURITIBA_THROW("Scene folder %s does not exist", pDirName); 			


				if (0 != dir) {

					int count = 0;
					while ((ent = readdir (dir)) != 0) {
						char file [1024];

#ifdef CURITIBA_PLATFORM_WIN32
						sprintf (file, "%s\\%s", (char *)FileUtil::GetFullPath(ProjectLoader::m_Path, pDirName).c_str(), ent->d_name);
#else
						sprintf (file, "%s/%s", pDirName, ent->d_name);						
#endif
						try {
							curitiba::Curitiba::getInstance()->loadAsset (file, pName,s);
						}
						catch(std::string &s) {
							closedir(dir);
							throw(s);
						}
						++count;
					}
					if (count < 3 )
						CURITIBA_THROW("Scene folder %s is empty", pDirName); 			

				closedir (dir);
				}
			}
		}
	}
}


/* ----------------------------------------------------------------
Specification of the atomic semantics:

		<atomics>
			<atomic id=0 semantics="Red Pixels"/>
			...
		</atomics>

Each atomic must have an id and a name.
----------------------------------------------------------------- */

void
ProjectLoader::loadAtomicSemantics(TiXmlHandle handle) 
{
	TiXmlElement *pElem;

	pElem = handle.FirstChild ("atomics").FirstChild ("atomic").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		const char *pName = pElem->Attribute ("semantics");
		int id;
		int failure = pElem->QueryIntAttribute("id", &id);

		if (failure) {
			CURITIBA_THROW("Atomic has no id, in file %s", ProjectLoader::m_File.c_str());
		}

		if (0 == pName) {
			CURITIBA_THROW("Atomic %d has no semantics, in file %s", id, ProjectLoader::m_File.c_str());
		}

		SLOG("Atomic : %d %s", id, pName);

		IRenderer::addAtomic(id,pName);
	} 
}


/* ----------------------------------------------------------------
Specification of the viewport:

		<viewports>
			<viewport name="MainViewport" fixed="false">
				<bgcolor r="0.0" g="0.0" b="0.3" />
				<geometry x=".66" y="0" width=".33" height=".33" />
			</viewport>
			...
		</viewports>

bgcolor is optional, if not specified it will be black
fixed is optional, if not specified it will be false
Geometry can be relative or absolute, if values are smaller than 1
it is assumed to be relative.
----------------------------------------------------------------- */

void
ProjectLoader::loadViewports(TiXmlHandle handle) 
{
	TiXmlElement *pElem;
	Viewport *v;

	pElem = handle.FirstChild ("viewports").FirstChild ("viewport").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		const char *pName = pElem->Attribute ("name");
		const char *pFixed = pElem->Attribute ("fixed");
		const char *pRatio = pElem->Attribute ("ratio");

		if (0 == pName) {
			CURITIBA_THROW("Viewport has no name, in file %s", ProjectLoader::m_File.c_str());
		}

		SLOG("Viewport : %s", pName);

		TiXmlElement *pElemAux = 0;
		pElemAux = pElem->FirstChildElement ("bgcolor");

		if (0 == pElemAux) {
			// no color is specified
			v = curitiba::Curitiba::getInstance()->createViewport(pName, vec4(0.0f, 0.0f, 0.0f, 1.0f));				
		}
		else {
			// color for viewport

			float r, g, b;

			if (TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("r", &r) || 
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("g", &g) ||
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("b", &b)) {

				CURITIBA_THROW("Background color error in viewport %s, in file %s", pName, ProjectLoader::m_File.c_str());			
			}
			v = curitiba::Curitiba::getInstance()->createViewport (pName, vec4 (r, g, b, 1.0f));
		}

		bool value = false;
		if (0 != pFixed) {
			value = !strcmp (pFixed, "true");
		}
		v->setFixed (value);

		if (value && !pRatio) {


			pElemAux = pElem->FirstChildElement ("geometry");

			if (0 == pElemAux) {
				CURITIBA_THROW("No geometry found for fixed viewport %s, in file %s", pName, ProjectLoader::m_File.c_str());
			}
			
			float x, y, width, height;

			if (TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("x", &x) || 
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("y", &y) ||
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("width", &width) ||
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("height", &height)){

				CURITIBA_THROW("Geometry definition error in viewport %s, in file %s", pName, ProjectLoader::m_File.c_str());					
			}
			
			v->setOrigin (x, y);
			v->setSize (width, height);
		}
		else if (value && pRatio) {
		
			float x, y, width,height;
			float r = curitiba::system::textutil::ParseFloat(pRatio);

			pElemAux = pElem->FirstChildElement ("geometry");

			if (0 == pElemAux) {
				CURITIBA_THROW("No geometry found for fixed viewport %s, in file %s", pName, ProjectLoader::m_File.c_str());
			}
		
			if (TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("x", &x) || 
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("y", &y) ||
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("width", &width) ){

				CURITIBA_THROW("Geometry definition error in viewport %s, in file %s", pName, ProjectLoader::m_File.c_str());					
			}
			if (TIXML_SUCCESS == pElemAux->QueryFloatAttribute ("height", &height))
				CURITIBA_THROW("Height ignored in viewport %s in file %s", pName,ProjectLoader::m_File.c_str());

			v->setOrigin (x, y);
			v->setSize (width, 0);
			v->setRatio(r);
		
		}
	} //End of Viewports
}


/* ----------------------------------------------------------------
Specification of the cameras:

		<cameras>
			<camera name="MainCamera" type="perspective">
				<viewport>MainViewport</viewport>
				<ortho left="-1.0" right="1.0" bottom="-1.0" top="1.0" near="-30.0" far="10.0" />
				<perspective fov="60.0" near="1.0" far="1000.0" />
				<position x="-240.0" y="180.0" z="-330" />
				<view x="0.54" y="-0.37" z="0.75" />
				<up x="0.0" y="1.0" z="0.0" />
			</camera>
			...
		</cameras>

type is optional and can be either "perspective" or "ortho", 
	if not specified it will be "perspective"

viewport is optional, if specified indicats the name of a previously defined viewport
	otherwise the default viewport ( a full screen viewport) will be used.
	TODO: must check if the viewport already exists

ortho and perspective, depending on the type of the camera, one of them must be defined.
position is optional, if not specified it will be (0.0 0.0, 0.0)
view is optional, if not specified it will be (0.0, 0.0, -1.0)
up is optional, if not defined it will be (0.0, 1.0, 0.0)
----------------------------------------------------------------- */

void
ProjectLoader::loadCameras(TiXmlHandle handle) 
{
	TiXmlElement *pElem;

	pElem = handle.FirstChild ("cameras").FirstChild ("camera").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		char defType[12] = "perspective";
		const char *pName = pElem->Attribute ("name");

		if (0 == pName) {
			CURITIBA_THROW("Camera has no name, in file %s", ProjectLoader::m_File.c_str());
		}

		SLOG("Camera: %s", pName);

		if (RENDERMANAGER->hasCamera(pName))
			CURITIBA_THROW("Camera %s is already defined, in file %s", pName, ProjectLoader::m_File.c_str());

		Camera *aNewCam = RENDERMANAGER->getCamera (pName);

		const char *pType = pElem->Attribute ("type");
		if (0 == pType) {
			pType = defType;
		}


		TiXmlElement *pElemAux = 0;

		pElemAux = pElem->FirstChildElement ("viewport");
		Viewport *v = 0;
		//if (0 == pElemAux) {
		//	v = curitiba::Curitiba::getInstance()->getDefaultViewport ();
		//} else {
			// CHECK IF EXISTS
			// MOVE VIEWPORTS TO RENDERMANAGER

			v = curitiba::Curitiba::getInstance()->getViewport (pElemAux->GetText());
			if (!v)
				CURITIBA_THROW("Viewport %s is not defined, in file %s", pElemAux->GetText(), ProjectLoader::m_File.c_str());

			aNewCam->setViewport (v);
		//}

		


		if (0 == strcmp (pType, "perspective")) {
			pElemAux = pElem->FirstChildElement ("perspective");
			
			float fov, nearPlane, farPlane;

			if (TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("fov", &fov) || 
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("near", &nearPlane) ||
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("far", &farPlane)) {

				CURITIBA_THROW("Perspective definition error for camera %s in file %s", pName, ProjectLoader::m_File.c_str());
			}
	
			aNewCam->setPerspective (fov, nearPlane, farPlane);
		} else {
			if (0 == strcmp (pType, "ortho")) {
				pElemAux = pElem->FirstChildElement ("ortho");

				float left, right, bottom, top, nearPlane, farPlane;

				if (TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("left", &left) || 
					TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("right", &right) ||
					TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("bottom", &bottom) ||
					TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("top", &top) || 
					TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("near", &nearPlane) ||
					TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("far", &farPlane)) {

					CURITIBA_THROW("Ortho definition error for camera %s in file %s", pName, ProjectLoader::m_File.c_str());
				}
				
				aNewCam->setOrtho (left, right, bottom, top, nearPlane, farPlane);
			}
		}

		vec3 pos, view, up;

		pElemAux = pElem->FirstChildElement ("position");
		if (0 == pElemAux) {
			pos.set (0.0f, 0.0f, 0.0f);
		} else {
			float x, y, z;

			if (TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("x", &x) || 
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("y", &y) ||
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("z", &z)) {
				
					CURITIBA_THROW("Position definition error for camera %s in file %s", pName, ProjectLoader::m_File.c_str());
			}

			pos.set (x, y, z);
		}
		pElemAux = pElem->FirstChildElement ("view");
		if (0 == pElemAux) {
			view.set (0.0f, 0.0f, -1.0f);
		} else {
			float x, y, z;

			if (TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("x", &x) || 
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("y", &y) ||
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("z", &z)) {

				CURITIBA_THROW("View definition error for camera %s in file %s", pName, ProjectLoader::m_File.c_str());
			}

			view.set (x, y, z);
		}
		pElemAux = pElem->FirstChildElement ("up");
		if (0 == pElemAux) {
			up.set (0.0f, 1.0f, 0.0f);
		} else {
			float x, y, z;

			if (TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("x", &x) || 
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("y", &y) ||
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("z", &z)) {

				CURITIBA_THROW("Up definition error for camera %s in file %s", pName, ProjectLoader::m_File.c_str());
			}
			up.set (x, y, z);
		}
		aNewCam->setCamera (pos, view, up);
	} //End of Cameras
}


/* ----------------------------------------------------------------
Specification of the lights:

		<lights>
			<light name="Sun" type="directional">
				<position x="0.0" y="0.0" z="0.0" />
				<direction x="0.0" y="0.0" z="-1.0" />
				<color r="1.0" g="1.0" b="1.0" />
				<ambient r="0.2", g="0.2", b="0.2" />
			</light>
			...
		</lights>

type is optional and can only be "directional" 
	TODO: Must add point and spotlights

position is optional, if not specified it will be (0.0 0.0, .0.)
direction is optional, if not specified it will be (0.0, 0.0, -1.0)
color is optional, if not defined it will be (1.0, 1.0, 1.0)
	TODO: must add ambient color
----------------------------------------------------------------- */
void 
ProjectLoader::loadLights(TiXmlHandle handle) 
{
	TiXmlElement *pElem;

	pElem = handle.FirstChild ("lights").FirstChild ("light").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		const char *pName = pElem->Attribute ("name");
		const char *pClass = pElem->Attribute("class"); 
		
		if (0 == pName) 
			CURITIBA_THROW("Light has no name in file %s", ProjectLoader::m_File.c_str());

		SLOG("Light: %s", pName);

		Light *l;

		if (RENDERMANAGER->hasLight(pName))
			CURITIBA_THROW("Light %s is already defined, in file %s", pName, ProjectLoader::m_File.c_str());

		if (0 == pClass) {
			l = RENDERMANAGER->getLight (pName);
		} else{
			l = RENDERMANAGER->getLight (pName, pClass);
		}

		int type;

		if (TIXML_SUCCESS != pElem->QueryIntAttribute ("type", &type)) {
			type = (int)Light::DIRECTIONAL;
		}

		vec3 pos, dir;
		vec4 color,ambient;
		TiXmlElement *pElemAux;

		pElemAux = pElem->FirstChildElement ("position");
		if (0 == pElemAux) {
			pos.set (0.0f, 0.0f, 0.0f);
		} else {
			float x, y, z;

			if (TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("x", &x) || 
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("y", &y) ||
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("z", &z)) {
				
					CURITIBA_THROW("Position definition error for light %s in file %s", pName, ProjectLoader::m_File.c_str());
			}
			pos.set (x, y, z);
		}

		pElemAux = pElem->FirstChildElement ("direction");
		if (0 == pElemAux) {
			dir.set (0.0f, 0.0f, -1.0f);
		} else {
			float x, y, z;

			if (TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("x", &x) || 
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("y", &y) ||
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("z", &z)) {

					CURITIBA_THROW("Direction definition error for light %s in file %s", pName, ProjectLoader::m_File.c_str());
			}
			dir.set (x, y, z);
		}

		pElemAux = pElem->FirstChildElement ("color");
		if (0 == pElemAux) {
			color.set (1.0f, 1.0f, 1.0f, 1.0f);
		} else {
			float r, g, b;

			if (TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("r", &r) || 
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("g", &g) ||
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("b", &b)) {
					
					CURITIBA_THROW("Color definition error for light %s in file %s", pName, ProjectLoader::m_File.c_str());
			}
			color.set (r, g, b, 1.0f);
		}	

		pElemAux = pElem->FirstChildElement ("ambient");
		if (0 == pElemAux) {
			color.set (1.0f, 1.0f, 1.0f, 1.0f);
		} else {
			float r, g, b;

			if (TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("r", &r) || 
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("g", &g) ||
				TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("b", &b)) {
					
					CURITIBA_THROW("Ambient color definition error for light %s in file %s", pName, ProjectLoader::m_File.c_str());
			}
			ambient.set (r, g, b, 1.0f);
		}	

		int enabled;
		pElemAux = pElem->FirstChildElement ("enabled");
		if (0 == pElemAux) {
			enabled = 1;
		} else if (TIXML_SUCCESS != pElemAux->QueryIntAttribute ("value", &enabled)) {

			CURITIBA_THROW("Enabled definition error for light %s in file %s", pName, ProjectLoader::m_File.c_str());
		}
		l->init (pos, dir, color, enabled, (Light::LightType)type);	
		l->setProp(Light::AMBIENT, ambient);
	}//End of lights
}


/* ----------------------------------------------------------------
Specification of the assets:

	<assets>
		<scenes>
			...
		</scenes>
		<viewports>
			...
		</viewports>
		<cameras>
			...
		</cameras>
		<lights>
			...
		</lights>
		<materiallibs>
			<mlib filename="..\mlibs\vision.mlib"/>
			<mlib filename="..\mlibs\quadMaterials.mlib"/>
		</materiallibs>
	</assets>


----------------------------------------------------------------- */


void
ProjectLoader::loadAssets (TiXmlHandle &hRoot, std::vector<std::string>  &matLibs)
{
	TiXmlElement *pElem;
	TiXmlHandle handle (hRoot.FirstChild ("assets").Element());

	loadScenes(handle);
	loadViewports(handle);
	loadCameras(handle);
	loadLights(handle);	
	loadEvents(handle);
	loadAtomicSemantics(handle);
	
	pElem = handle.FirstChild ("materialLibs").FirstChild ("mlib").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		const char *pFilename = pElem->Attribute ("filename");

		if (0 == pFilename) {
			CURITIBA_THROW("No file specified for material lib in file %s", ProjectLoader::m_File.c_str());
		}

		try {
			SLOG("Loading Material Lib from file : %s", FileUtil::GetFullPath(ProjectLoader::m_Path,pFilename).c_str());
			loadMatLib(FileUtil::GetFullPath(ProjectLoader::m_Path,pFilename));
		}
		catch(std::string &s) {
			throw(s);
		}
	}
}


/*-------------------------------------------------------------*/
/*                   PASS        ELEMENTS                      */
/*-------------------------------------------------------------*/

/* -----------------------------------------------------------------------------
CAMERAS

	<camera>testCamera</camera>

Specifies a previously defined camera (in the assets part of the file)
-----------------------------------------------------------------------------*/

void 
ProjectLoader::loadPassCamera(TiXmlHandle hPass, Pass *aPass) 
{
	TiXmlElement *pElem;

	pElem = hPass.FirstChild ("camera").Element();
	if (0 == pElem) {
		CURITIBA_THROW("No camera element found in pass: %s", aPass->getName().c_str());
	}

	if (!RENDERMANAGER->hasCamera(pElem->GetText()))
		CURITIBA_THROW("Camera %s is not defined, in pass: %s", pElem->GetText(), aPass->getName().c_str());

	aPass->setCamera (pElem->GetText());
}

/* -----------------------------------------------------------------------------
LIGHTS

	<lights>
		<light>Sun</light>
	</lights>

Specifies a previously defined light (in the assets part of the file)
-----------------------------------------------------------------------------*/

void 
ProjectLoader::loadPassLights(TiXmlHandle hPass, Pass *aPass) 
{
	TiXmlElement *pElem;

	pElem = hPass.FirstChild ("lights").FirstChild ("light").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		const char *pName = pElem->GetText();

		if (0 == pName) {
			CURITIBA_THROW("Light has no name in pass: %s", aPass->getName().c_str());
		}
		if (!RENDERMANAGER->hasLight(pName))
			CURITIBA_THROW("Light %s is not defined, in pass: %s", pName, aPass->getName().c_str());

		aPass->addLight (pName);
	}//End of lights
}



/* -----------------------------------------------------------------------------
SCENES

	<scenes>
		<scene>MainScene</scene>
		...
	</scenes>

Specifies a previously defined scene (in the assets part of the file)
-----------------------------------------------------------------------------*/



void 
ProjectLoader::loadPassScenes(TiXmlHandle hPass, Pass *aPass) 
{
	TiXmlElement *pElem;

	pElem = hPass.FirstChild ("scenes").FirstChild ("scene").Element();
	if (0 == pElem) {
		CURITIBA_THROW("No Scene element found in pass: %s", aPass->getName().c_str());
	}
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {

		if (!RENDERMANAGER->hasScene(pElem->GetText()))
				CURITIBA_THROW("Scene %s is not defined, in pass: %s", pElem->GetText(),aPass->getName().c_str());

		aPass->addScene (pElem->GetText());
	} //End of scenes
}


/* -----------------------------------------------------------------------------
CLEAR DEPTH AND COLOR

	<color clear=true />
	<depth clear=true clearValue=1.0 test=true write=true/>

	<stencil clear=true clearValue=0 test=true mask=255>
		<stencilFunc func=ALWAYS ref=1 mask=255/>
		<stencilOp sfail=KEEP dfail=KEEP dpass=KEEP />
	</stencil>

By default these fields will be true and can be omitted.
-----------------------------------------------------------------------------*/

void 
ProjectLoader::loadPassClearDepthAndColor(TiXmlHandle hPass, Pass *aPass)
{
	TiXmlElement *pElem;

	// Clear Color and Depth
	pElem = hPass.FirstChild ("depth").Element();
	if (0 != pElem) {
		float vf;

		const char *pEnable = pElem->Attribute ("test");
		if (pEnable != NULL) {
			if (!strcmp(pEnable, "false"))
				aPass->setProp(IRenderer::DEPTH_ENABLE, false);
			else
				aPass->setProp(IRenderer::DEPTH_ENABLE, true);
		}
		const char *pClear = pElem->Attribute ("clear");
		if (pClear != NULL) {
			if (!strcmp(pClear, "false"))
				aPass->setProp(IRenderer::DEPTH_CLEAR, false);
			else
				aPass->setProp(IRenderer::DEPTH_CLEAR, true);
		}
		const char *pWrite = pElem->Attribute ("write");
		if (pWrite != NULL) {
			if (!strcmp(pWrite, "false"))
				aPass->setProp(IRenderer::DEPTH_MASK, false);
			else
				aPass->setProp(IRenderer::DEPTH_MASK, true);
		}
		if (TIXML_SUCCESS == pElem->QueryFloatAttribute ("clearValue",&vf))
			aPass->setDepthClearValue(vf);

		const char *pFunc = pElem->Attribute("func");
		if (pFunc) {
			int enumFunc = IState::translateStringToFuncEnum(pFunc);
			if (enumFunc != -1)
				aPass->setDepthFunc((IState::Func)enumFunc);
		}

	}

	pElem = hPass.FirstChild ("stencil").Element();
	if (0 != pElem) {
		float vf;int vi;
		const char *pEnable = pElem->Attribute ("test");
		if (pEnable != NULL) {
			if (!strcmp(pEnable, "false"))
				aPass->setProp(IRenderer::STENCIL_ENABLE, false);
			else
				aPass->setProp(IRenderer::STENCIL_ENABLE, true);
		}
		const char *pClear = pElem->Attribute ("clear");
		if (pClear != NULL) {
			if (!strcmp(pClear, "false"))
				aPass->setProp(IRenderer::STENCIL_CLEAR, false);
			else
				aPass->setProp(IRenderer::STENCIL_CLEAR, true);
		}
		if (TIXML_SUCCESS == pElem->QueryFloatAttribute ("clearValue",&vf))
			aPass->setStencilClearValue(vf);
		if (TIXML_SUCCESS == pElem->QueryIntAttribute ("maskValue",&vi))
			aPass->setStencilMaskValue(vf);

		TiXmlElement *pElemAux = pElem->FirstChildElement("stencilFunc");
		int ref, mask;
		if (pElemAux != NULL) {
			const char *pFunc = pElemAux->Attribute("func");
			if ((pFunc != NULL) && (IState::translateStringToFuncEnum(pFunc) != -1) &&
				(TIXML_SUCCESS == pElemAux->QueryIntAttribute ("ref",&ref)) &&
				(TIXML_SUCCESS == pElemAux->QueryIntAttribute ("mask",&mask)) && (mask >= 0))

					aPass->setStencilFunc((IState::Func)IState::translateStringToFuncEnum(pFunc), 
											ref, (unsigned int)mask);
		}
		pElemAux = pElem->FirstChildElement("stencilOp");
		if (pElemAux != NULL) {

			const char *pSFail = pElemAux->Attribute("sfail");
			const char *pDFail = pElemAux->Attribute("dfail");
			const char *pDPass = pElemAux->Attribute("dpass");
			if (pSFail != NULL && pDFail != NULL && pDPass != NULL && 
				IRenderer::translateStringToStencilOp(pSFail) != -1 &&
				IRenderer::translateStringToStencilOp(pDFail) != -1 &&
				IRenderer::translateStringToStencilOp(pDPass) != -1 )

					aPass->setStencilOp(
							(IRenderer::StencilOp)IRenderer::translateStringToStencilOp(pSFail),
							(IRenderer::StencilOp)IRenderer::translateStringToStencilOp(pDFail),
							(IRenderer::StencilOp)IRenderer::translateStringToStencilOp(pDPass));

		}
	}

	pElem = hPass.FirstChild ("color").Element();
	if (0 != pElem) {
		const char *pEnable = pElem->Attribute ("clear");
		if (pEnable != NULL) {
			if (!strcmp(pEnable, "false"))
				aPass->setProp(IRenderer::COLOR_CLEAR, false);
			else
				aPass->setProp(IRenderer::COLOR_CLEAR, true);
		}
	}
}

/* -----------------------------------------------------------------------------
VIEWPORTS

	<viewport>SmallViewport2</viewport>				

If a viewport is defined it will replace the viewport of the passe's camera
-----------------------------------------------------------------------------*/

void 
ProjectLoader::loadPassViewport(TiXmlHandle hPass, Pass *aPass)
{
	TiXmlElement *pElem;


	pElem = hPass.FirstChild ("viewport").Element();
	if (0 != pElem) {

		// CHECK IF EXISTS
		aPass->setViewport (curitiba::Curitiba::getInstance()->getViewport (pElem->GetText()));
	}
}
		
	

/* -----------------------------------------------------------------------------
TEXTURE - Used in quad pass

	<texture name="bla" fromLibrary="bli" />				

Should be an existing texture which will be displayed in the quad, 
usually it is a render target
-----------------------------------------------------------------------------*/

void 
ProjectLoader::loadPassTexture(TiXmlHandle hPass, Pass *aPass)
{
	TiXmlElement *pElem;


	pElem = hPass.FirstChild ("texture").Element();
	if (0 != pElem) {
		const char *pName = pElem->Attribute ("name");
		const char *pLib = pElem->Attribute("fromLibrary");
		
		if (!pName )
			CURITIBA_THROW("Texture without name in pass: %s", aPass->getName().c_str());
		if (!pLib) 
			sprintf(m_pFullName, "%s", pName);
		else
			sprintf(m_pFullName, "%s::%s", pLib, pName);

		if (!RESOURCEMANAGER->hasTexture(m_pFullName))
				CURITIBA_THROW("Texture %s is not defined, in pass: %s", m_pFullName,aPass->getName().c_str());


		Material *srcMat, *dstMat;
		srcMat = MATERIALLIBMANAGER->getDefaultMaterial("__Quad");
		dstMat = srcMat->clone();
		dstMat->attachTexture(0,m_pFullName);
		MATERIALLIBMANAGER->addMaterial(aPass->getName(),dstMat);
		aPass->remapMaterial ("__Quad", aPass->getName(), "__Quad");

		/*Material *srcMat = new Material();
		srcMat->clone()
		srcMat->setName("__Quad");
		srcMat->getColor().setDiffuse(1.0,1.0,1.0,1.0);

		srcMat->attachTexture(0,m_pFullName);
		MATERIALLIBMANAGER->addMaterial(aPass->getName(),srcMat);

		aPass->remapMaterial ("__Quad", aPass->getName(), "__Quad");*/

	}
}
/* -----------------------------------------------------------------------------
PARAMS

	<params>
		<param name = "SplitIndex" int="2" />
	</params>

Some passes may take other parameters which can be specified in here.
The available types so far are int and float.
-----------------------------------------------------------------------------*/

// CHECK IF EXISTS (HAVE TO CHECK PARAMS)

void 
ProjectLoader::loadPassParams(TiXmlHandle hPass, Pass *aPass)
{
	TiXmlElement *pElem;
	int vi;
	float vf;
	std::string s;

	pElem = hPass.FirstChild("params").FirstChild("param").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {

		const char *pName = pElem->Attribute ("name");
		
		if (!pName)
			CURITIBA_THROW("Param without name in pass: %s", aPass->getName().c_str());

		const char *pValue = pElem->Attribute ("string");
		if (TIXML_SUCCESS == pElem->QueryIntAttribute ("int",&vi))
			aPass->setParam(pName,vi);
		else if (TIXML_SUCCESS == pElem->QueryFloatAttribute ("float",&vf))
			aPass->setParam(pName,vf);
		else if (pValue != NULL)
				aPass->setParam(pName,pValue);
		else {
			CURITIBA_THROW("Param %s without value in pass: %s", pName, aPass->getName().c_str());
		}
	}
}

/* -----------------------------------------------------------------------------
RENDERTARGET

	<rendertarget name = "deferredOuput" fromLibrary="testMaterials" />

	or

	<rendertarget sameas="pass2"/>
-----------------------------------------------------------------------------*/

void 
ProjectLoader::loadPassRenderTargets(TiXmlHandle hPass, Pass *aPass,std::map<std::string, Pass*> passMapper)
{
	TiXmlElement *pElem;


	pElem = hPass.FirstChild ("rendertarget").Element();

	if (0 != pElem) {
		
		const char* pSameAs = pElem->Attribute ("sameas");
		const char *pName = pElem->Attribute("name");
		const char *pLib = pElem->Attribute("fromLibrary");

		if (0 != pSameAs) {
			if (passMapper.count (pSameAs) > 0) {
				aPass->setRenderTarget (passMapper[pSameAs]->getRenderTarget());
			} else {
				CURITIBA_THROW("Render Target Definition in %s: Pass %s is not defined", aPass->getName().c_str(), pSameAs);
			}
		} 
		else if (0 != pName && 0 != pLib) {
	
			sprintf(m_pFullName, "%s::%s", pLib, pName);
			RenderTarget *rt = RESOURCEMANAGER->getRenderTarget(m_pFullName);
			if (rt != NULL)
				aPass->setRenderTarget(rt);
			else
				CURITIBA_THROW("Render Target %s is not defined in material lib %s, in pass %s", pName, pLib, aPass->getName().c_str(), pSameAs);
			
		}
		else {
			CURITIBA_THROW("Render Target Definition error in %s", aPass->getName().c_str());
		}
	}
}


/* ----------------------------------------------------------------------------

	OPTIX SETTINGS

	<optixEntryPoint>
		<optixProgram type="RayGen" file="optix/common.ptx" proc="pinhole_camera"/> 
		<optixProgram type="Exception" file="optix/common.ptx" proc="exception"/> 
	</optixEntryPoint>

	<optixDefaultMaterial>
		<optixProgram type="Closest_Hit" ray="Phong" file="optix/common.ptx" proc="shade"/> 
		<optixProgram type="Miss" ray="Phong" file="optix/common.ptx" proc="background"/> 

		<optixProgram type="Any_Hit" ray="Shadow" file="optix/common.ptx" proc="shadows"/> 
	</optixDefaultMaterial>

	// for closest and any hit rays
	<optixMaterialMap>
		<optixMap to="Vidro">
			<optixProgram type="Any_Hit" ray="Phong" file="optix/common.ptx" proc="keepGoing"/> 
			<optixProgram type="Any_Hit" ray="Shadow" file="optix/common.ptx" proc="keepGoingShadow"/> 
		</optixMap>	
	</optixMaterialMap>

	// for input buffers, as in texture buffers
	<optixInput>
		<buffer var="bla" texture="lib::texname" />
	</optixInput>

	// selects which vertex attributes are fed to optix
	<optixVertexAttributes>
		<attribute name="position"/>
	</optixVertexAttributes>

	// Geometry and bonuding box programs
	<optixGeometryProgram> 
			<optixProgram type="Geometry_Intersect" file="optix/common.ptx" proc="bla"/> 
			<optixProgram type="Bounding_Box" file="optix/common.ptx" proc="bla2"/> 
	</optixGeometryProgram>

	// optix output buffers
	// note: not required for render targets
	<optixOutput>
		<buffer var="dataBuffer" texture="Optix Ray Tracer Render Target::dataBuffer" />
	</optixOutput>

	// For material attributes. Tells optix which attributes to use from the materials
	<optixMaterialAttributes>
		<valueof optixVar="diffuse" type="CURRENT" context="COLOR" component="DIFFUSE" />
		<valueof optixVar="ambient" type="CURRENT" context="COLOR" component="AMBIENT" />
		<valueof uniform="texCount"	type="CURRENT" context="TEXTURE" component="COUNT" />
	</optixMaterialAttributes>

	// For globl attributes, i.e. attributes that remain constant per frame
	<optixGlobalAttributes>
		<valueof optixVar="lightDir" type="CURRENT" context="LIGHT" id=0 component="DIRECTION" />
	</optixGlobalAttributes>


-------------------------------------------------------------------------------*/
#ifdef CURITIBA_OPTIX

void
ProjectLoader::loadPassOptixSettings(TiXmlHandle hPass, Pass *aPass) {

	TiXmlElement *pElem, *pElemAux, *pElemAux2;
	PassOptix *p = (PassOptix *)aPass;

	pElem = hPass.FirstChild("optixEntryPoint").FirstChildElement("optixProgram").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {

		const char *pType = pElem->Attribute ("type");
		const char *pFile = pElem->Attribute ("file");
		const char *pProc = pElem->Attribute ("proc");
		
		if (!pType || (0 != strcmp(pType, "RayGen") && 0 != strcmp(pType, "Exception")))
			CURITIBA_THROW("Invalid Optix Entry Point Type in pass %s", aPass->getName().c_str());

		if (!pFile)
			CURITIBA_THROW("Missing Optix Entry Point File in pass %s", aPass->getName().c_str());

		if (!pProc)
			CURITIBA_THROW("Missing Optix Entry Point Proc in pass %s", aPass->getName().c_str());

		if (!strcmp(pType, "RayGen"))
			p->setOptixEntryPointProcedure(curitiba::render::optixRender::OptixRenderer::RAY_GEN, pFile, pProc);
		else
			p->setOptixEntryPointProcedure(curitiba::render::optixRender::OptixRenderer::EXCEPTION, pFile, pProc);
	}
	pElem = hPass.FirstChild("optixDefaultMaterial").FirstChildElement("optixProgram").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {

		const char *pType = pElem->Attribute ("type");
		const char *pFile = pElem->Attribute ("file");
		const char *pProc = pElem->Attribute ("proc");
		const char *pRay  = pElem->Attribute ("ray");
		
		if (!pType || (0 != strcmp(pType, "Closest_Hit") && 0 != strcmp(pType, "Any_Hit")  && 0 != strcmp(pType, "Miss")))
			CURITIBA_THROW("Invalid Optix Default Material Proc Type in pass %s", aPass->getName().c_str());

		if (!pFile)
			CURITIBA_THROW("Missing Optix Default Material Proc File in pass %s", aPass->getName().c_str());

		if (!pProc)
			CURITIBA_THROW("Missing Optix Default Material Proc in pass %s", aPass->getName().c_str());

		if (!pRay)
			CURITIBA_THROW("Missing Optix Default Material Ray in pass %s", aPass->getName().c_str());
		
		if (!strcmp("Closest_Hit", pType)) 
			p->setDefaultMaterialProc(curitiba::render::optixRender::OptixMaterialLib::CLOSEST_HIT, pRay, pFile, pProc);
		else if (!strcmp("Any_Hit", pType)) 
			p->setDefaultMaterialProc(curitiba::render::optixRender::OptixMaterialLib::ANY_HIT, pRay, pFile, pProc);
		else if (!strcmp("Miss", pType)) 
			p->setDefaultMaterialProc(curitiba::render::optixRender::OptixMaterialLib::MISS, pRay, pFile, pProc);
	}

	pElem = hPass.FirstChild("optixMaterialMap").FirstChildElement("optixMap").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {

		const char *pName = pElem->Attribute("to");

		pElemAux = pElem->FirstChildElement("optixProgram");
		for ( ; 0 != pElemAux; pElemAux = pElemAux->NextSiblingElement()) {

		const char *pType = pElemAux->Attribute ("type");
		const char *pFile = pElemAux->Attribute ("file");
		const char *pProc = pElemAux->Attribute ("proc");
		const char *pRay  = pElemAux->Attribute ("ray");

		if (!pType || (0 != strcmp(pType, "Closest_Hit") && 0 != strcmp(pType, "Any_Hit")))
			CURITIBA_THROW("Invalid Optix Material Proc Type in pass %s", aPass->getName().c_str());

		if (!pFile)
			CURITIBA_THROW("Missing Optix Material Proc File in pass %s", aPass->getName().c_str());

		if (!pProc)
			CURITIBA_THROW("Missing Optix Material Proc in pass %s", aPass->getName().c_str());

		if (!pRay)
			CURITIBA_THROW("Missing Optix Material Ray in pass %s", aPass->getName().c_str());
		
		if (!strcmp("Closest_Hit", pType)) 
			p->setMaterialProc(pName, curitiba::render::optixRender::OptixMaterialLib::CLOSEST_HIT, pRay, pFile, pProc);
		else if (!strcmp("Any_Hit", pType)) 
			p->setMaterialProc(pName, curitiba::render::optixRender::OptixMaterialLib::ANY_HIT, pRay, pFile, pProc);
		}
	}

	pElem = hPass.FirstChild("optixInput").FirstChildElement("buffer").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {

		const char *pVar = pElem->Attribute ("var");
		const char *pTexture = pElem->Attribute ("texture");
		
		if (!pVar)
			CURITIBA_THROW("Optix Variable required in Input Definition, in pass %s", aPass->getName().c_str());

		if (!pTexture)
			CURITIBA_THROW("Missing texture in Optix Input Definitiont, in pass %s", aPass->getName().c_str());

		if (!RESOURCEMANAGER->hasTexture(pTexture))
				CURITIBA_THROW("Texture %s is not defined, in pass: %s", pTexture,aPass->getName().c_str());

		p->setInputBuffer(pVar, pTexture);
	}

	pElem = hPass.FirstChild("optixOutput").FirstChildElement("buffer").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {

		const char *pVar = pElem->Attribute ("var");
		const char *pTexture = pElem->Attribute ("texture");
		
		if (!pVar)
			CURITIBA_THROW("Optix Variable required in Input Definition, in pass %s", aPass->getName().c_str());

		if (!pTexture)
			CURITIBA_THROW("Missing texture in Optix Input Definitiont, in pass %s", aPass->getName().c_str());

		if (!RESOURCEMANAGER->hasTexture(pTexture))
				CURITIBA_THROW("Texture %s is not defined, in pass: %s", pTexture,aPass->getName().c_str());

		p->setOutputBuffer(pVar, pTexture);
	}

	pElem = hPass.FirstChild("optixGeometryProgram").FirstChildElement("optixProgram").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {

		const char *pType = pElem->Attribute ("type");
		const char *pFile = pElem->Attribute ("file");
		const char *pProc = pElem->Attribute ("proc");
		
		if (!pType || (0 != strcmp(pType, "Geometry_Intersection") && 0 != strcmp(pType, "Bounding_Box")))
			CURITIBA_THROW("Invalid Optix Geometry Program in pass %s", aPass->getName().c_str());

		if (!pFile)
			CURITIBA_THROW("Missing Optix Geometry Program File in pass %s", aPass->getName().c_str());

		if (!pProc)
			CURITIBA_THROW("Missing Optix Geometry Program Proc in pass %s", aPass->getName().c_str());

		if (!strcmp(pType, "Geometry_Intersection"))
			p->setGeometryIntersectProc(pFile, pProc);
		else
			p->setBoundingBoxProc(pFile, pProc);
	}

	pElem = hPass.FirstChild("optixVertexAttributes").FirstChildElement("attribute").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {

		const char *pType = pElem->Attribute ("name");
		unsigned int vi = VertexData::getAttribIndex(pType);
		if (!pType || (VertexData::MaxAttribs == vi ))
			CURITIBA_THROW("Invalid Optix Vertex Attribute in pass %s", aPass->getName().c_str());
	
		p->addVertexAttribute(vi);
	}

	PassOptix *po = (PassOptix *)aPass;
	pElemAux2 = hPass.FirstChild("optixMaterialAttributes").FirstChildElement("valueof").Element();
	for ( ; 0 != pElemAux2; pElemAux2 = pElemAux2->NextSiblingElement()) {
	

		const char *pUniformName = pElemAux2->Attribute ("optixVar");
		const char *pComponent = pElemAux2->Attribute ("component");
		const char *pContext = pElemAux2->Attribute("context");
		const char *pType = pElemAux2->Attribute("type");
		//const char *pId = pElemAux2->Attribute("id");

		if (0 == pUniformName) {
			CURITIBA_THROW("No optix variable name, in pass %s", aPass->getName().c_str());
		}
		if (0 == pType) {
			CURITIBA_THROW("No type found for optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
		}
		if (0 == pContext) {
			CURITIBA_THROW("No context found for optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
		}
		if (0 == pComponent) {
			CURITIBA_THROW("No component found for optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
		}
		if (!ProgramValue::Validate(pType, pContext, pComponent))
			CURITIBA_THROW("Optix variable %s is not valid, in pass %s", pUniformName, aPass->getName().c_str());

		int id = 0;
		if (((strcmp(pContext,"LIGHT") == 0) || (0 == strcmp(pContext,"TEXTURE"))) &&  (0 != strcmp(pComponent,"COUNT"))) {
			if (TIXML_SUCCESS != pElemAux2->QueryIntAttribute ("id", &id))
				CURITIBA_THROW("No id found for optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
			if (id < 0)
				CURITIBA_THROW("id must be non negative, in optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
		}
		std::string s(pType);
		if (s == "TEXTURE") {
			if (!RESOURCEMANAGER->hasTexture(pContext)) {
				CURITIBA_THROW("Texture %s is not defined, in pass %s", pContext, aPass->getName().c_str());
			}
			else
				po->addMaterialAttribute (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));
		}

		else if (s == "CAMERA") {
			// Must consider that a camera can be defined internally in a pass, example:lightcams
			/*if (!RENDERMANAGER->hasCamera(pContext))
				CURITIBA_THROW("Camera %s is not defined in the project file", pContext);*/
			po->addMaterialAttribute (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));
		}
		else if (s == "LIGHT") {
			if (!RENDERMANAGER->hasLight(pContext))
				CURITIBA_THROW("Light %s is not defined in the project file", pContext);
			po->addMaterialAttribute (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));

		}
		else
			po->addMaterialAttribute (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));

			//sprintf(m_pFullName, "%s(%s,%s)",pType,pContext,pComponent );

			
	}
		pElemAux2 = hPass.FirstChild("optixMaterialAttributes").FirstChildElement("valueof").Element();
	for ( ; 0 != pElemAux2; pElemAux2 = pElemAux2->NextSiblingElement()) {
	

		const char *pUniformName = pElemAux2->Attribute ("optixVar");
		const char *pComponent = pElemAux2->Attribute ("component");
		const char *pContext = pElemAux2->Attribute("context");
		const char *pType = pElemAux2->Attribute("type");
		//const char *pId = pElemAux2->Attribute("id");

		if (0 == pUniformName) {
			CURITIBA_THROW("No optix variable name, in pass %s", aPass->getName().c_str());
		}
		if (0 == pType) {
			CURITIBA_THROW("No type found for optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
		}
		if (0 == pContext) {
			CURITIBA_THROW("No context found for optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
		}
		if (0 == pComponent) {
			CURITIBA_THROW("No component found for optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
		}
		if (!ProgramValue::Validate(pType, pContext, pComponent))
			CURITIBA_THROW("Optix variable %s is not valid, in pass %s", pUniformName, aPass->getName().c_str());

		int id = 0;
		if (((strcmp(pContext,"LIGHT") == 0) || (0 == strcmp(pContext,"TEXTURE"))) &&  (0 != strcmp(pComponent,"COUNT"))) {
			if (TIXML_SUCCESS != pElemAux2->QueryIntAttribute ("id", &id))
				CURITIBA_THROW("No id found for optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
			if (id < 0)
				CURITIBA_THROW("id must be non negative, in optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
		}
		std::string s(pType);
		if (s == "TEXTURE") {
			if (!RESOURCEMANAGER->hasTexture(pContext)) {
				CURITIBA_THROW("Texture %s is not defined, in pass %s", pContext, aPass->getName().c_str());
			}
			else
				po->addMaterialAttribute (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));
		}

		else if (s == "CAMERA") {
			// Must consider that a camera can be defined internally in a pass, example:lightcams
			/*if (!RENDERMANAGER->hasCamera(pContext))
				CURITIBA_THROW("Camera %s is not defined in the project file", pContext);*/
			po->addMaterialAttribute (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));
		}
		else if (s == "LIGHT") {
			if (!RENDERMANAGER->hasLight(pContext))
				CURITIBA_THROW("Light %s is not defined in the project file", pContext);
			po->addMaterialAttribute (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));

		}
		else
			po->addMaterialAttribute (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));

			//sprintf(m_pFullName, "%s(%s,%s)",pType,pContext,pComponent );

			
	}
	pElemAux2 = hPass.FirstChild("optixGlobalAttributes").FirstChildElement("valueof").Element();
	for ( ; 0 != pElemAux2; pElemAux2 = pElemAux2->NextSiblingElement()) {
	

		const char *pUniformName = pElemAux2->Attribute ("optixVar");
		const char *pComponent = pElemAux2->Attribute ("component");
		const char *pContext = pElemAux2->Attribute("context");
		const char *pType = pElemAux2->Attribute("type");
		//const char *pId = pElemAux2->Attribute("id");

		if (0 == pUniformName) {
			CURITIBA_THROW("No optix variable name, in pass %s", aPass->getName().c_str());
		}
		if (0 == pType) {
			CURITIBA_THROW("No type found for optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
		}
		if (0 == pContext) {
			CURITIBA_THROW("No context found for optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
		}
		if (0 == pComponent) {
			CURITIBA_THROW("No component found for optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
		}
		if (!ProgramValue::Validate(pType, pContext, pComponent))
			CURITIBA_THROW("Optix variable %s is not valid, in pass %s", pUniformName, aPass->getName().c_str());

		int id = 0;
		if (((strcmp(pContext,"LIGHT") == 0) || (0 == strcmp(pContext,"TEXTURE"))) &&  (0 != strcmp(pComponent,"COUNT"))) {
			if (TIXML_SUCCESS != pElemAux2->QueryIntAttribute ("id", &id))
				CURITIBA_THROW("No id found for optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
			if (id < 0)
				CURITIBA_THROW("id must be non negative, in optix variable %s, in pass %s", pUniformName, aPass->getName().c_str());
		}
		std::string s(pType);
		if (s == "TEXTURE") {
			if (!RESOURCEMANAGER->hasTexture(pContext)) {
				CURITIBA_THROW("Texture %s is not defined, in pass %s", pContext, aPass->getName().c_str());
			}
			else
				po->addGlobalAttribute (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));
		}

		else if (s == "CAMERA") {
			// Must consider that a camera can be defined internally in a pass, example:lightcams
			/*if (!RENDERMANAGER->hasCamera(pContext))
				CURITIBA_THROW("Camera %s is not defined in the project file", pContext);*/
			po->addGlobalAttribute (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));
		}
		else if (s == "LIGHT") {
			if (!RENDERMANAGER->hasLight(pContext))
				CURITIBA_THROW("Light %s is not defined in the project file", pContext);
			po->addGlobalAttribute (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));

		}
		else
			po->addGlobalAttribute (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));

			//sprintf(m_pFullName, "%s(%s,%s)",pType,pContext,pComponent );

			
	}


}

#endif
/* ----------------------------------------------------------------------------

	END OPTIX SETTINGS

-------------------------------------------------------------------------------*/


/* -----------------------------------------------------------------------------
MATERIAL LIBRAY RENDERTARGET DEFINITION

	<rendertargets>
		<size width="2048" height="2048" />
		<colors>
			<color name="offscreenrender" format="RGBA" type="UNSIGNED_BYTE" />
		</colors>
		<depth name="shadowMap1" noreadandwrite="true" internalFormat="DEPTH_COMPONENT24" type="FLOAT"  />
		<depthStencil name="bli" />
	</rendertargets>

The names of both color and depth RTs will be available for other passes to use
	as textures

There can be multiple color, but only one depth.
depth and depthStencil can't be bothdefined 
Depth and Color can be omitted, but at least one of them must be present.


-----------------------------------------------------------------------------*/

void 
ProjectLoader::loadMatLibRenderTargets(TiXmlHandle hRoot, MaterialLib *aLib, std::string path)
{
	TiXmlElement *pElem;
	RenderTarget *m_RT;
	int rtWidth, rtHeight, rtSamples = 0; 

	pElem = hRoot.FirstChild ("rendertargets").FirstChild ("rendertarget").Element();
	for ( ; 0 != pElem; pElem=pElem->NextSiblingElement()) {
		const char *pRTName = pElem->Attribute ("name");

		if (!pRTName)
			CURITIBA_THROW("Render Target has no name, in material lib %s", aLib->getName());


		// Render Target size
		TiXmlElement *pElemSize;
		pElemSize = pElem->FirstChildElement ("size");
			
		if (0 != pElemSize) {

			
			if (TIXML_SUCCESS != pElemSize->QueryIntAttribute ("width", &rtWidth)) {
				CURITIBA_THROW("No width element found in render target %s, in material lib %s", pRTName, aLib->getName().c_str());
			}

			if (TIXML_SUCCESS != pElemSize->QueryIntAttribute ("height",  &rtHeight)) {
				CURITIBA_THROW("No height element found in render target %s, in material lib %s", pRTName, aLib->getName().c_str());
			}

			sprintf(m_pFullName, "%s::%s", aLib->getName().c_str(), pRTName);
			m_RT = RESOURCEMANAGER->createRenderTarget (m_pFullName, rtWidth, rtHeight);	
			//aPass->setRTSize (rtWidth, rtHeight);
			

		} 
		else {
			CURITIBA_THROW("No size element found in render target %s, in material lib %s", pRTName, aLib->getName().c_str());
		} //End of  rendertargets size

		// Render Target multisampling
		TiXmlElement *pElemMS;
		pElemMS = pElem->FirstChildElement ("multisampling");

		
		if (0 != pElemMS) {
		
			if (TIXML_SUCCESS != pElemMS->QueryIntAttribute ("samples", &rtSamples)) {
				CURITIBA_THROW("Number of samples not found in render target %s, in material lib %s", pRTName, aLib->getName().c_str());
			}

			m_RT->setSampleCount(rtSamples);

		} // End of rendertargets multisampling

		// Render Target clear values
		TiXmlElement *pElemClear;
		pElemClear = pElem->FirstChildElement ("clear");
			
		if (0 != pElemClear) {

			float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f; 
			pElemClear->QueryFloatAttribute ("r", &r);
			pElemClear->QueryFloatAttribute ("g", &g);
			pElemClear->QueryFloatAttribute ("b", &b);
			pElemClear->QueryFloatAttribute ("a", &a);

			m_RT->setClearValues(r,g,b,a);
		} //End of  rendertargets clear


		TiXmlElement *pElemDepth;
		pElemDepth = pElem->FirstChildElement ("depth");

		if (0 != pElemDepth) {
			const char *pNameDepth = pElemDepth->Attribute ("name");
			const char *pNoReadAndWrite = pElemDepth->Attribute ("noreadandwrite");

			const char *internalFormat = pElemDepth->Attribute("internalFormat");
			const char *type = pElemDepth->Attribute("type");
			const char *format = pElemDepth->Attribute("format");

			int samples = 1;
			int multisample = pElemDepth->QueryIntAttribute("samples", &samples);

				bool noReadAndWrite = false;

			if (0 != pNoReadAndWrite) {
				noReadAndWrite = !strcmp (pNoReadAndWrite, "true");
			}

			if (0 == pNameDepth) {
				CURITIBA_THROW("Depth rendertarget has no name, in render target %s,in material lib: %s", pRTName, aLib->getName().c_str());							
			}

			if (internalFormat == 0) {
				CURITIBA_THROW("Depth rendertarget %s has no internal format, in render target %s, in material lib: %s", pNameDepth, pRTName, aLib->getName().c_str());
			}

			if (type == 0 && rtSamples == 0) {
				CURITIBA_THROW("Depth rendertarget %s has no type, in render target %s, in material lib: %s", pNameDepth, pRTName, aLib->getName().c_str());
			}

			if (format == 0 && rtSamples == 0) {
				CURITIBA_THROW("Depth rendertarget %s has no format, in render target %s, in material lib: %s", pNameDepth, pRTName, aLib->getName().c_str());
			}

			/*if (!Texture::isValidInternalFormat(internalFormat))
				CURITIBA_THROW("Depth rendertarget %s internal format %s is invalid in pass: %s", pNameRT, internalFormat, aPass->getName().c_str());

			if (!Texture::isValidFormat(format))
				CURITIBA_THROW("Depth rendertarget %s format %s is invalid in pass: %s", pNameRT, format, aPass->getName().c_str());
*/
						
			sprintf(m_pFullName, "%s::%s", aLib->getName().c_str(), pNameDepth);
			m_RT->addDepthTarget (m_pFullName, internalFormat, format, type, noReadAndWrite);
		}

		//pElemDepth = pElem->FirstChildElement ("stencil");

		//if (0 != pElemDepth) {
		//	const char *pNameDepth = pElemDepth->Attribute ("name");

		//	if (0 == pNameDepth) {
		//		CURITIBA_THROW("Stencil rendertarget has no name, in render target %s,in material lib: %s", pRTName, aLib->getName().c_str());							
		//	}
		//				
		//	sprintf(m_pFullName, "%s::%s", aLib->getName().c_str(), pNameDepth);
		//	m_RT->addStencilTarget (m_pFullName);
		//}

		pElemDepth = pElem->FirstChildElement ("depthStencil");

		if (0 != pElemDepth) {
			const char *pNameDepth = pElemDepth->Attribute ("name");

			if (0 == pNameDepth) {
				CURITIBA_THROW("Depth/Stencil rendertarget has no name, in render target %s,in material lib: %s", pRTName, aLib->getName().c_str());							
			}
						
			sprintf(m_pFullName, "%s::%s", aLib->getName().c_str(), pNameDepth);
			m_RT->addDepthStencilTarget (m_pFullName);
		}

		TiXmlElement *pElemColor;	
		TiXmlNode *pElemColors;
		pElemColors = pElem->FirstChild("colors");
		if (pElemColors != NULL) {
			pElemColor = pElemColors->FirstChildElement("color");

			for ( ; 0 != pElemColor; pElemColor = pElemColor->NextSiblingElement()) {

				const char *pNameColor = pElemColor->Attribute ("name");
				const char *internalFormat = pElemColor->Attribute("internalFormat");
				const char *type = pElemColor->Attribute("type");
				const char *format = pElemColor->Attribute("format");

				int samples = 1;
				int multisample = pElemColor->QueryIntAttribute("samples", &samples);

				if (0 == pNameColor) {
					CURITIBA_THROW("Color rendertarget has no name, in render target %s,in material lib: %s", pRTName, aLib->getName().c_str());							
				}

				if (internalFormat == 0) {
					CURITIBA_THROW("Color rendertarget %s has no internal format, in render target %s, in material lib: %s", pNameColor, pRTName, aLib->getName().c_str());
				}

				if (type == 0  && rtSamples == 0) {
					CURITIBA_THROW("Color rendertarget %s has no type, in render target %s, in material lib: %s", pNameColor, pRTName, aLib->getName().c_str());
				}

				if (format == 0 && rtSamples == 0) {
					CURITIBA_THROW("Color rendertarget %s has no format, in render target %s, in material lib: %s", pNameColor, pRTName, aLib->getName().c_str());
				}

				if (!Texture::isValidInternalFormat(internalFormat))
					CURITIBA_THROW("Color rendertarget %s internal format is invalid, in render target %s, in material lib: %s", pNameColor, pRTName, aLib->getName().c_str());

				if (!Texture::isValidFormat(format))
					CURITIBA_THROW("Color rendertarget %s format is invalid, in render target %s, in material lib: %s", pNameColor, pRTName, aLib->getName().c_str());

				sprintf(m_pFullName, "%s::%s", aLib->getName().c_str(), pNameColor);
					
				m_RT->addColorTarget (m_pFullName, internalFormat, format,type);
			}//End of rendertargets color
		}
		SLOG("Render Target : %s width:%d height:%d samples:%d", pRTName, rtWidth, rtHeight, rtSamples);

		if (!m_RT->checkStatus()) {
			CURITIBA_THROW("Render target is not OK, in render target %s,in material lib: %s", pRTName, aLib->getName().c_str());							

		}
	}//End of rendertargets


}

/* -----------------------------------------------------------------------------
EVENTS

THEY NEED TO BE CHECKED IF EVERY ASSET REQUIRED IS DEFINED	

-----------------------------------------------------------------------------*/

void
ProjectLoader::loadEvents(TiXmlHandle handle)
{
	TiXmlElement *pElem;


	pElem = handle.FirstChild ("sensors").FirstChild ("sensor").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		const char *pName = pElem->Attribute ("name");
		const char *pClass = pElem->Attribute("class"); 
		const char *pObject = pElem->Attribute("object"); 

		if (0 == pName) {
			CURITIBA_THROW("Sensor has no name in file %s", ProjectLoader::m_File.c_str());
		}

		if (0 == pClass) {
			CURITIBA_THROW("Sensor %d has no class in file %s", pName, ProjectLoader::m_File.c_str());
		}


		if (!SensorFactory::validate(pClass)) {
			CURITIBA_THROW("Invalid Class for Sensor %s, in file %s", pName, ProjectLoader::m_File.c_str());
		}

		Sensor *s;
		std::string propName;
		int iVal;
		float fVal;
		vec3 v3Val;
		float xVal, yVal, zVal;
		s = EVENTMANAGER->getSensor(pName,pClass);
		TiXmlElement *pElemAux;

		for (unsigned int i = 0; i < Sensor::COUNT_PROPTYPE; ++i) {

			switch (i) {
				case Sensor::BOOL: 
					for (unsigned int prop = 0; prop < s->getBoolPropCount(); ++prop) {
	
						propName = s->getBoolPropNames(prop);
						pElemAux = pElem->FirstChildElement(propName.c_str());
						if (pElemAux) {
							if (TIXML_SUCCESS != pElemAux->QueryIntAttribute ("value", &iVal)) {
								CURITIBA_THROW("Sensor %s def error, field %s, in file %s", pName, propName.c_str(), ProjectLoader::m_File.c_str());
							}
							else
								s->setBool(prop, (iVal != 0));
						}
					}
					break;
				case Sensor::FLOAT:
					for (unsigned int prop = 0; prop < s->getFloatPropCount(); ++prop) {
	
						propName = s->getFloatPropNames(prop);
						pElemAux = pElem->FirstChildElement(propName.c_str());
						if (pElemAux) {
							if (TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("value", &fVal)) {
								CURITIBA_THROW("Sensor %s def error, field %s, in file %s", pName, propName.c_str(), ProjectLoader::m_File.c_str());
							}
							else
								s->setFloat(prop, fVal);
						}
					}
					break;
				case Sensor::VEC3:
					for (unsigned int prop = 0; prop < s->getVec3PropCount(); ++prop) {
	
						propName = s->getVec3PropNames(prop);
						pElemAux = pElem->FirstChildElement(propName.c_str());
						if (pElemAux) {
							if (TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("x", &xVal) ||
								TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("y", &yVal) ||
								TIXML_SUCCESS != pElemAux->QueryFloatAttribute ("z", &zVal)) {
								CURITIBA_THROW("Sensor %s def error, field %s, in file %s", pName, propName.c_str(), ProjectLoader::m_File.c_str());
							}
							else {
								v3Val.set(xVal, yVal, zVal);
								s->setVec3(prop, v3Val);
							}
						}
					}
					break;
			}
		}
		s->init();
	}	
	// End of Sensors

	//Begin of Interpolators //Marta
	pElem = handle.FirstChild ("interpolators").FirstChild ("interpolator").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		const char *pName = pElem->Attribute ("name");
		const char *pClass = pElem->Attribute("class"); 

		if (0 == pName) {
			CURITIBA_THROW("Interpolator has no name in file %s", ProjectLoader::m_File.c_str());
		}
		if (0 == pClass) {
			CURITIBA_THROW("Interpolator %s has no class in file %s", pName, ProjectLoader::m_File.c_str());
		}

		Interpolator *in= EVENTMANAGER->getInterpolator(pName, pClass);
		if (in==0) {
			CURITIBA_THROW("Class definition error for interpolator %s in file %s ",pName, ProjectLoader::m_File.c_str());
		}

		TiXmlHandle hKeyFrames(pElem);
		TiXmlElement *pElemAux;
		pElemAux = hKeyFrames.FirstChild("keyFrames").FirstChild("keyFrame").Element();
		for (; 0 != pElemAux; pElemAux = pElemAux->NextSiblingElement()) {
		
				float key = 0.0, x=0.0f, y=0.0f, z=0.0f, w=0.0f;
				pElemAux->QueryFloatAttribute ("key", &key);
				pElemAux->QueryFloatAttribute ("x", &x);
				pElemAux->QueryFloatAttribute ("y", &y);
				pElemAux->QueryFloatAttribute ("z", &z);
				pElemAux->QueryFloatAttribute ("w", &w);

				in->addKeyFrame(key,vec4(x,y,z,w));
		
		}
	}

	// End of Interpolators


	SceneObject *o;
	pElem = handle.FirstChild ("moveableobjects").FirstChild ("moveableobject").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		const char *pName = pElem->Attribute ("name");
		const char *pObject = pElem->Attribute("object");

		if (0 == pName) 
			CURITIBA_THROW("MoveableObject has no name in file %s ", ProjectLoader::m_File.c_str());

		if (0 == pObject) {
			o=0;
		}

		o = RENDERMANAGER->getScene("MainScene")->getSceneObject(pObject); // substituir o MainScene, pode ter mais do q uma Cena?

		curitiba::event_::ObjectAnimation *oa= new curitiba::event_::ObjectAnimation(pName, o);

		//in->init((char *) pName, o, (char *)pKey, (char *)pKeyValue); 
		
	}
	// End of MoveableObjects

	////Begin of routes //Marta
	pElem = handle.FirstChild ("routes").FirstChild ("route").Element();

	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {

		const char *pName = pElem->Attribute ("name");
		const char *pSender = pElem->Attribute ("sender");
		const char *pReceiver = pElem->Attribute ("receiver");
		const char *pIn = pElem->Attribute ("eventIn");
		const char *pOut = pElem->Attribute ("eventOut");

		if (0 == pName) 
			CURITIBA_THROW("Route has no name in file %s", ProjectLoader::m_File.c_str());

		if (0 == pSender) 
			CURITIBA_THROW("Route %s has no sender in file %s", pName, ProjectLoader::m_File.c_str());

		if (0 == pReceiver) 
			CURITIBA_THROW("Route %s has no receiver in file %s", pName, ProjectLoader::m_File.c_str());

		if (0 == pIn) 
			CURITIBA_THROW("Route %s has no eventIn in file %s", pName, ProjectLoader::m_File.c_str());

		if (0 == pOut) 
			CURITIBA_THROW("Route %s has no eventOut in file %s", pName, ProjectLoader::m_File.c_str());		

		Route *r= EVENTMANAGER->getRoute(pName);
		r->init((char *) pName, (char *)pSender,(char *)pReceiver,(char *)pOut,(char *)pIn);
	}
	// End of routes
}

/* -----------------------------------------------------------------------------
MATERIAL MAPS

	MAP ALL TO ONE
	<materialmaps>
		<map fromMaterial="*" toLibrary="quadMaterials" toMaterial="flat-with-shadow" />
	</materialmaps>

	OR 

	MAP INDIVIDUAL
	<materialmaps>
		<map fromMaterial="quad" toLibrary="quadMaterials" toMaterial="quadpass2" />
	</materialmaps>

The field toLibrary indicates a previously defined material library in the assets part.

-----------------------------------------------------------------------------*/

void 
ProjectLoader::loadPassMaterialMaps(TiXmlHandle hPass, Pass *aPass)
{
	TiXmlElement *pElem;
	std::string library;

	pElem = hPass.FirstChild ("materialMaps").FirstChild ("map").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		const char *pFromMaterial = pElem->Attribute ("fromMaterial");
		
		const char *pToLibrary = pElem->Attribute ("toLibrary");

		if (pToLibrary == 0)
			library = DEFAULTMATERIALLIBNAME;
		else
			library = pToLibrary;

		const char *pToMaterial = pElem->Attribute ("toMaterial");

		//int pPriority; 
		//if (TIXML_SUCCESS != pElem->QueryIntAttribute("fromPriority",&pPriority))
		//	pPriority = -1;

		//if (-1 != pPriority && pFromMaterial)

		//	CURITIBA_THROW("A Material Map can be used for materials or priorities, not both simultaneously. In pass: %s", aPass->getName().c_str());

		if (/*-1 != pPriority && */(pToMaterial == 0)) {

		    CURITIBA_THROW("Material map error in pass : %s", aPass->getName().c_str());
		}
		if (/*pPriority == -1 && */( 
		    (0 != pFromMaterial && 0 == pToMaterial) ||
		    (0 == pFromMaterial && 0 != pToMaterial))) {
		  
		    CURITIBA_THROW("Material map error in pass: %s", aPass->getName().c_str());
		}

		//if (pPriority != -1) {

		//	if (MATERIALLIBMANAGER->hasMaterial(library, pToMaterial))
		//		aPass->remapAllFromPriority(pPriority, library, pToMaterial);
		//	else
		//		CURITIBA_THROW("Material Map Error, destination material (%s,%s) is not defined, in  pass: %s", library, pToMaterial,aPass->getName().c_str());
		//}
		
		else if (0 == pFromMaterial) {
			if (MATERIALLIBMANAGER->hasLibrary(library))
				aPass->remapAll (library);
			else
				CURITIBA_THROW("Material Map Error, destination library %s is not defined, in  pass: %s", library.c_str(), aPass->getName().c_str());
		}
		else if (0 == strcmp (pFromMaterial, "*")) {
			if (MATERIALLIBMANAGER->hasMaterial(library, pToMaterial))
				aPass->remapAll (library, pToMaterial);
			else
				CURITIBA_THROW("Material Map Error, destination material (%s,%s) is not defined, in  pass: %s", library.c_str(), pToMaterial,aPass->getName().c_str());
		}
		else {
			if (MATERIALLIBMANAGER->hasMaterial(library, pToMaterial))
				aPass->remapMaterial (pFromMaterial, library, pToMaterial);
			else
				CURITIBA_THROW("Material Map Error, destination material (%s,%s) is not defined, in  pass: %s", library.c_str(), pToMaterial,aPass->getName().c_str());
		}
	} //End of map

}

/* -----------------------------------------------------------------------------
SHADERMAPS

	<shadermaps>
		<attach shader="perpixel-color" fromLibrary="bla" toMaterial="c"/>
	</shadermaps>

Creates a new material in the pass material lib, cloning the default's
library material "c" and adding the shader to it

NOTE: This would more usefull if allowed to attach shaders and textures and states
to a material
-----------------------------------------------------------------------------*/
//
//void 
//ProjectLoader::loadPassShaderMaps(TiXmlHandle hPass, Pass *aPass)
//{
//	TiXmlElement *pElem;
//
//	pElem = hPass.FirstChild("shadermaps").FirstChild("attach").Element();
//	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
//		const char *pShader = pElem->Attribute ("shader");
//		const char *pLib = pElem->Attribute("fromLibrary");
//		const char *pToMaterial = pElem->Attribute ("toMaterial");
//
//		if (0 == pShader || 0 == pToMaterial || 0 == pLib) {
//		  CURITIBA_THROW("Shader map error in pass: %s", aPass->getName().c_str());
//		}
//
//		sprintf(m_pFullName, "%s::%s", pLib, pShader);
//		if (!RESOURCEMANAGER->hasProgram(m_pFullName))
//			CURITIBA_THROW("Shader %s is not defined in lib %s, in pass: %s", pShader, pLib,aPass->getName().c_str());
//
//		//aPass->attachShader(pShader, pToMaterial);
//
//		MaterialLib *aLib = MATERIALLIBMANAGER->getLib(aPass->getName());
//		MaterialLib *defLib = MATERIALLIBMANAGER->getLib(DEFAULTMATERIALLIBNAME);
//
//		std::vector<std::string> *names = defLib->getMaterialNames(pToMaterial);
//		std::vector<std::string>::iterator iter;
//
//		for(iter = names->begin(); iter != names->end(); ++iter) {
//
//			std::string name = *iter;
//			Material *srcMat = defLib->getMaterial(name);
//			Material *dstMat = srcMat->clone();
//			MATERIALLIBMANAGER->addMaterial(aPass->getName(), dstMat);
//			dstMat->attachProgram(m_pFullName);
//
//			aPass->remapMaterial (name, aPass->getName(), name);
//		}
//		delete names;
//	}
//
//}

/* -----------------------------------------------------------------------------
STATEMAPS

	<statemaps>
		<set inMaterial="Grade*" state="Grades" />
	</statemaps>

Creates a new material in the passe's library with the same name, and the same 
values of the original material, and attaches a state to it.

An * can be used at the end of the material, and it works as a wildcard.

-----------------------------------------------------------------------------*/

//void 
//ProjectLoader::loadPassStateMaps(TiXmlHandle hPass, Pass *aPass)
//{
//	TiXmlElement *pElem;
//
//	pElem = hPass.FirstChild ("statemaps").FirstChild ("set").Element();
//	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
//		const char *pInMaterial = pElem->Attribute ("inMaterial");
//		const char *pState = pElem->Attribute ("state");
//		const char *pLib = pElem->Attribute ("fromLibrary");
//
//		if (0 == pInMaterial || 0 == pState || 0 == pLib) {
//		  CURITIBA_THROW("State map error in pass: %s", aPass->getName().c_str());
//		}
//
//		sprintf(m_pFullName, "%s::%s", pLib, pState);
//		if (!RESOURCEMANAGER->hasState(m_pFullName))
//			CURITIBA_THROW("State %s is not defined in lib %s, in pass: %s", pState, pLib,aPass->getName().c_str());
//
//		MaterialLib *aLib = MATERIALLIBMANAGER->getLib(aPass->getName());
//		MaterialLib *defLib = MATERIALLIBMANAGER->getLib(DEFAULTMATERIALLIBNAME);
//
//		std::vector<std::string> *names = defLib->getMaterialNames(pInMaterial);
//		std::vector<std::string>::iterator iter;
//
//		for(iter = names->begin(); iter != names->end(); ++iter) {
//
//			std::string name = *iter;
//			Material *srcMat = defLib->getMaterial(name);
//			Material *dstMat = srcMat->clone();
//			MATERIALLIBMANAGER->addMaterial(aPass->getName(), dstMat);
//			dstMat->setState(RESOURCEMANAGER->getState(m_pFullName));
//
//			aPass->remapMaterial (name, aPass->getName(), name);
//		}
//		delete names;
//	} //End of map
//}


/* -----------------------------------------------------------------------------
MAPS - Allow the setting of individual settings of loaded materials

	<injectionMaps>
		<map toMaterial="Grade*">
			<state name="Grades"  fromLibrary="bli"/>
			<shader fromMaterial="bla" fromLibrary="bli" />
			<color fromMaterial="bla" fromLibrary="bli" ambient=true diffuse=false emission=false specular=false shininess=false />
			<textures>
				<texture name="tex" fromLibrary="bli" toUnit="0" >
					<depthCompare mode="COMPARE_REF_TO_TEXTURE" func="LEQUAL" />
					<filtering min="LINEAR" mag="LINEAR" />
				</texture>
			</textures>

		</map>
	</injectionMaps>

Creates a new material in the passe's library with the same name, and the same 
values of the original material, replaces the defined properties.

An * can be used at the end of the material, and it works as a wildcard.

-----------------------------------------------------------------------------*/

void 
ProjectLoader::loadPassInjectionMaps(TiXmlHandle hPass, Pass *aPass)
{
	TiXmlElement *pElem, *pElemAux;

	pElem = hPass.FirstChild ("injectionMaps").FirstChild ("map").Element();
	for ( ; pElem != NULL; pElem = pElem->NextSiblingElement()) {
	
		const char* pMat = pElem->Attribute("toMaterial");

		if (0 == pMat)
			CURITIBA_THROW("Map error: a name is required for the material, in pass: %s", aPass->getName().c_str());
	
		MaterialLib *defLib = MATERIALLIBMANAGER->getLib(DEFAULTMATERIALLIBNAME);
		std::vector<std::string> *names = defLib->getMaterialNames(pMat);

		if (names->size() == 0)
			CURITIBA_THROW("No materials match %s in map definition, in pass: %s", pMat, aPass->getName().c_str());

		Material *dstMat;

		std::vector<std::string>::iterator iter;
		for(iter = names->begin(); iter != names->end(); ++iter) {

			std::string name = *iter;
			Material *srcMat = defLib->getMaterial(name);
			dstMat = srcMat->clone();
			MATERIALLIBMANAGER->addMaterial(aPass->getName(), dstMat);

			aPass->remapMaterial (name, aPass->getName(), name);
		}


		pElemAux = pElem->FirstChildElement("state");
		if (pElemAux) {
	
			const char *pState = pElemAux->Attribute ("name");
			const char *pLib = pElemAux->Attribute ("fromLibrary");

			if (0 == pState || 0 == pLib) {
			  CURITIBA_THROW("State map error in pass: %s", aPass->getName().c_str());
			}

			sprintf(m_pFullName, "%s::%s", pLib, pState);
			if (!RESOURCEMANAGER->hasState(m_pFullName))
				CURITIBA_THROW("State %s is not defined in lib %s, in pass: %s", pState, pLib,aPass->getName().c_str());
		
			std::vector<std::string>::iterator iter;
			for(iter = names->begin(); iter != names->end(); ++iter) {

				std::string name = *iter;
				dstMat = MATERIALLIBMANAGER->getMaterial(aPass->getName(), name);
				dstMat->setState(RESOURCEMANAGER->getState(m_pFullName));
			}
		}
	
		pElemAux = pElem->FirstChildElement("shader");
		if (pElemAux) {

			const char *pMat = pElemAux->Attribute ("fromMaterial");
			const char *pLib = pElemAux->Attribute ("fromLibrary");

			if (0 == pMat || 0 == pLib) {
			  CURITIBA_THROW("State map error in pass: %s", aPass->getName().c_str());
			}

			if (!MATERIALLIBMANAGER->hasMaterial(pLib,pMat))
				CURITIBA_THROW("Material %s is not defined in lib %s, in pass: %s", pMat, pLib,aPass->getName().c_str());

			std::vector<std::string>::iterator iter;
			for(iter = names->begin(); iter != names->end(); ++iter) {

				std::string name = *iter;
				dstMat = MATERIALLIBMANAGER->getMaterial(aPass->getName(), name);
				dstMat->cloneProgramFromMaterial(MATERIALLIBMANAGER->getMaterial(pLib,pMat));
			}

		}

		TiXmlNode *pElemNode = pElem->FirstChild("textures");
		if (pElemNode) {
			pElemAux = pElemNode->FirstChildElement ("texture");
			for ( ; pElemAux != NULL; pElemAux = pElemAux->NextSiblingElement()) {
	
				const char *pName = pElemAux->Attribute ("name");
				const char *pLib = pElemAux->Attribute ("fromLibrary");
		

				if (0 == pName || 0 == pLib) {
				  CURITIBA_THROW("Texture map error in pass: %s", aPass->getName().c_str());
				}

				int unit;
				if (TIXML_SUCCESS != pElemAux->QueryIntAttribute("toUnit", &unit))
				  CURITIBA_THROW("Texture unit not specified in material map, in pass: %s", aPass->getName().c_str());

				sprintf(m_pFullName, "%s::%s", pLib, pName);
				if (!RESOURCEMANAGER->hasTexture(m_pFullName))
					CURITIBA_THROW("Texture %s is not defined in lib %s, in pass: %s", pName, pLib,aPass->getName().c_str());


				TiXmlElement *pElemAux2;
				const char *mode = NULL;
				const char *func = NULL;
				const char *min = NULL, *mag = NULL;
				pElemAux2 = pElemAux->FirstChildElement ("depthCompare");
				if (pElemAux2) {
					mode = pElemAux2->Attribute("mode");
					if (0 == mode){
						CURITIBA_THROW("Depth Compare definition error in library %s in pass %s", pLib,  aPass->getName().c_str());
					}

					func = pElemAux2->Attribute("func");
					if (0 == func){
						CURITIBA_THROW("Depth Compare  definition error in library %s in pass %s", pLib,  aPass->getName().c_str());
					}
				}

				pElemAux2 = pElemAux->FirstChildElement ("filtering");
				if (pElemAux2) {
					min = pElemAux2->Attribute("min");
					if (0 == min){
						CURITIBA_THROW("Filtering definition error in library %s in pass %s", pLib,  aPass->getName().c_str());
					}

					mag = pElemAux2->Attribute("mag");
					if (0 == mag){
						CURITIBA_THROW("Filtering  definition error in library %s in pass %s", pLib,  aPass->getName().c_str());
					}
				}




				Texture::TextureUnit texUnit = (Texture::TextureUnit)(Texture::TEXTURE0+unit);
				std::vector<std::string>::iterator iter;
				for(iter = names->begin(); iter != names->end(); ++iter) {

					std::string name = *iter;
					dstMat = MATERIALLIBMANAGER->getMaterial(aPass->getName(), name);
					dstMat->attachTexture(unit, RESOURCEMANAGER->getTexture(m_pFullName));
					if (mode) 
						dstMat->getTextureSampler(unit)->setTexProp(TextureSampler::TEXTURE_COMPARE_MODE, TextureSampler::translateStringToTexCompareModeEnum(mode));
					if (func)
						dstMat->getTextureSampler(unit)->setTexProp(TextureSampler::TEXTURE_COMPARE_FUNC, TextureSampler::translateStringToTexCompareFuncEnum(func));
					
					if (min)
						dstMat->getTextureSampler(unit)->setTexProp(TextureSampler::TEXTURE_MIN_FILTER, TextureSampler::translateStringToTexMinModeEnum(min));
					if (mag)
						dstMat->getTextureSampler(unit)->setTexProp(TextureSampler::TEXTURE_MAG_FILTER, TextureSampler::translateStringToTexMagModeEnum(mag));
					//if (mode) 
					//	dstMat->getState()->setTexProp( texUnit, IState::TEXTURE_COMPARE_MODE, IState::translateStringToTexCompareModeEnum(mode));
					//if (func)
					//	dstMat->getState()->setTexProp( texUnit, IState::TEXTURE_COMPARE_FUNC, IState::translateStringToTexCompareFuncEnum(func));
					//
					//if (min)
					//	dstMat->getState()->setTexProp( texUnit, IState::TEXTURE_MIN_FILTER, IState::translateStringToTexMinModeEnum(min));
					//if (mag)
					//	dstMat->getState()->setTexProp( texUnit, IState::TEXTURE_MAG_FILTER, IState::translateStringToTexMagModeEnum(mag));
				}
		
			}
		}


		pElemAux = pElem->FirstChildElement("color");
		if (pElemAux) {

			const char *pMat = pElemAux->Attribute ("fromMaterial");
			const char *pLib = pElemAux->Attribute ("fromLibrary");
			const char *pDiffuse = pElemAux->Attribute("diffuse");
			const char *pAmbient = pElemAux->Attribute("ambient");
			const char *pSpecular = pElemAux->Attribute("specular");
			const char *pEmission = pElemAux->Attribute("emission");
			const char *pShininess = pElemAux->Attribute("shininess");

			if (0 == pMat || 0 == pLib) {
			  CURITIBA_THROW("State map error in pass: %s", aPass->getName().c_str());
			}

			if (!MATERIALLIBMANAGER->hasMaterial(pLib,pMat))
				CURITIBA_THROW("Material %s is not defined in lib %s, in pass: %s", pMat, pLib,aPass->getName().c_str());

			std::vector<std::string>::iterator iter;
			for(iter = names->begin(); iter != names->end(); ++iter) {

				std::string name = *iter;
				dstMat = MATERIALLIBMANAGER->getMaterial(aPass->getName(), name);
				if (pAmbient && !strcmp("true",pAmbient))
					dstMat->getColor().setAmbient(MATERIALLIBMANAGER->getMaterial(pLib,pMat)->getColor().getAmbient());
				if (pDiffuse && !strcmp("true",pDiffuse))
					dstMat->getColor().setDiffuse(MATERIALLIBMANAGER->getMaterial(pLib,pMat)->getColor().getDiffuse());
				if (pSpecular && !strcmp("true",pSpecular))
					dstMat->getColor().setSpecular(MATERIALLIBMANAGER->getMaterial(pLib,pMat)->getColor().getSpecular());
				if (pEmission && !strcmp("true",pEmission))
					dstMat->getColor().setEmission(MATERIALLIBMANAGER->getMaterial(pLib,pMat)->getColor().getEmission());
				if (pShininess && !strcmp("true",pShininess))
					dstMat->getColor().setShininess(MATERIALLIBMANAGER->getMaterial(pLib,pMat)->getColor().getShininess());
			}

		}
		delete names;
	}
}

/* -----------------------------------------------------------------------------
PIPELINES   AND    PASSES     

	<pipelines>
		<pipeline name="shadow" default="true" defaultCamera="aName">
			<pass class="default" name="pass1">
				...
			</pass>
		</pipeline>
	</pipelines>

in the pipeline definition, if default is not present then the first pipeline
will be the default

in the pass definition if class is not present it will be "default"
-----------------------------------------------------------------------------*/
void
ProjectLoader::loadPipelines (TiXmlHandle &hRoot)
{
	TiXmlElement *pElem;
	TiXmlHandle handle (0);
	std::map<std::string, Pass*> passMapper;


	char activePipeline[256];

	memset (activePipeline, 0, 256);

	pElem = hRoot.FirstChild ("pipelines").FirstChild ("pipeline").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		const char *pNamePip = pElem->Attribute ("name");
		const char *pDefault = pElem->Attribute ("default");
		const char *pDefaultCamera = pElem->Attribute("defaultCamera");

		if (0 == pNamePip) 
			CURITIBA_THROW("Pipeline has no name");

		if (pDefaultCamera && !(RENDERMANAGER->hasCamera(pDefaultCamera)))
			CURITIBA_THROW("Camera %s, defined as default in pipeline %s, is not defined", pDefaultCamera, pNamePip);

		if (RENDERMANAGER->hasPipeline(pNamePip))
			CURITIBA_THROW("Pipeline %s is already defined in file %s", pNamePip, ProjectLoader::m_File.c_str());

		Pipeline *aPipeline = RENDERMANAGER->getPipeline (pNamePip);
		
		// if no default pipeline is set, then the first pipeline will be the default
		if (0 == strcmp (pDefault, "true")) {
			strcpy (activePipeline, pNamePip);
		}

		handle = TiXmlHandle (pElem);
		TiXmlElement *pElemPass;


		pElemPass = handle.FirstChild ("pass").Element();
		for ( ; 0 != pElemPass; pElemPass = pElemPass->NextSiblingElement()) {
			
			TiXmlHandle hPass (pElemPass);
			const char *pName = pElemPass->Attribute ("name");
			const char *pClass = pElemPass->Attribute ("class");

			if (0 == pName) 
				CURITIBA_THROW("Pass has no name in file %s", ProjectLoader::m_File.c_str());

			if (RENDERMANAGER->hasPass(pNamePip, pName))
				CURITIBA_THROW("Pass %s is defined more than once, in pipeline %s, in file %s", pName, pNamePip, ProjectLoader::m_File.c_str());

			Pass *aPass = 0;
			if (0 == pClass) {
				aPass = aPipeline->createPass(pName);
			} else {
				if (PassFactory::isClass(pClass))
					aPass = aPipeline->createPass (pName, pClass);
				else
					CURITIBA_THROW("Class %s is not available, in pass (%s,%s), in file %s", pClass, pNamePip, pName, ProjectLoader::m_File.c_str());

			}
			passMapper[pName] = aPass;
					
			if (0 != strcmp (pClass, "quad") && 0 != strcmp(pClass,"profiler")) {

				loadPassScenes(hPass,aPass);
				loadPassCamera(hPass,aPass);			
			}
			else
				loadPassTexture(hPass,aPass);
#ifdef CURITIBA_OPTIX
			if (0 == strcmp("optix", pClass))
				loadPassOptixSettings(hPass, aPass);
#endif
			loadPassParams(hPass, aPass);
			loadPassViewport(hPass, aPass);
			loadPassClearDepthAndColor(hPass, aPass);
			loadPassRenderTargets(hPass, aPass, passMapper);
			loadPassLights(hPass, aPass);

			loadPassInjectionMaps(hPass, aPass);
			loadPassMaterialMaps(hPass, aPass);
			//loadPassStateMaps(hPass, aPass);
			//loadPassShaderMaps(hPass, aPass);
		} //End of pass
		if (pDefaultCamera)
			aPipeline->setDefaultCamera(pDefaultCamera);

		//else
		//	aPipeline->setDefaultCamera(aPipeline->getLastPassCameraName());
	} //End of pipeline
	
	if (strlen (activePipeline) > 0) {
		RENDERMANAGER->setActivePipeline (activePipeline);
	} else {
		CURITIBA_THROW("No default pipeline");
	}
}

// ARF_ SEMANTICS CHECK::: I'M HERE
/* -----------------------------------------------------------------------------
TEXTURES

	<textures>
		<texture name="Grade_01_02_03" filename="../Texturas/AL01_Grade1.tif" />
		<cubeMap name="CMHouse" 
			filePosX="../TextureCubeMaps/cubemaphouse/cm_right.jpg"
			fileNegX="../TextureCubeMaps/cubemaphouse/cm_left.jpg"
			filePosY="../TextureCubeMaps/cubemaphouse/cm_top.jpg"
			fileNegY="../TextureCubeMaps/cubemaphouse/cm_bottom.jpg"
			filePosZ="../TextureCubeMaps/cubemaphouse/cm_front.jpg"
			fileNegZ="../TextureCubeMaps/cubemaphouse/cm_back.jpg"	
		/>
	</textures>

The paths may be relative to the material lib file, or absolute.
-----------------------------------------------------------------------------*/
void 
ProjectLoader::loadMatLibTextures(TiXmlHandle hRoot, MaterialLib *aLib, std::string path)
{
	TiXmlElement *pElem;
	pElem = hRoot.FirstChild ("textures").FirstChild ("texture").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		const char* pTextureName = pElem->Attribute ("name");
		const char* pFilename = pElem->Attribute ("filename");
		const char *internalFormat = pElem->Attribute("internalFormat");
		const char *type = pElem->Attribute("type");
		const char *format = pElem->Attribute("format");
		int mipmap=0;
		const char *pMipMap = pElem->Attribute("mipmap", &mipmap);



		if (0 == pTextureName) {
			CURITIBA_THROW("Texture has no name in Mat Lib %s", aLib->getName().c_str());
		} 

		sprintf(m_pFullName,"%s::%s", aLib->getName().c_str(), pTextureName);

		SLOG("Texture : %s", m_pFullName);

		if (RESOURCEMANAGER->hasTexture(m_pFullName))
			CURITIBA_THROW("Texture %s is already defined, in Mat Lib %s", m_pFullName, aLib->getName().c_str());

		if (0 == pFilename && 0 == internalFormat && 0 == type && 0 == format ) {
			CURITIBA_THROW("Incomplete texture %s definition Mat Lib %s", pTextureName, aLib->getName().c_str());
		}

		// empty texture
		if (0 == pFilename) {
			if (0 == internalFormat || 0 == type || 0 == format) {
				CURITIBA_THROW("Incomplete texture %s definition Mat Lib %s", pTextureName, aLib->getName().c_str());		
			}
			if (!Texture::isValidInternalFormat(internalFormat))
				CURITIBA_THROW("Texture %s internal format is invalid, in material lib: %s", pTextureName, aLib->getName().c_str());

			if (!Texture::isValidFormat(format))
				CURITIBA_THROW("Texture %s format is invalid, in material lib: %s", pTextureName, aLib->getName().c_str());

			int width, height;
			if (pElem->QueryIntAttribute("width", &width) || pElem->QueryIntAttribute("height", &height))
				CURITIBA_THROW("Texture %s dimensions are missing or invalid, in material lib: %s", pTextureName, aLib->getName().c_str());

			RESOURCEMANAGER->createTexture (m_pFullName, internalFormat,format, type, width, height);
		}

		else {
			bool mipmap = true;
			if (pMipMap != 0 && strcmp(pMipMap,"No") == 0)
				mipmap = false;

			RESOURCEMANAGER->addTexture (FileUtil::GetFullPath(path,pFilename), m_pFullName, mipmap);
		}
	}
	pElem = hRoot.FirstChild ("textures").FirstChild ("cubeMap").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		const char* pTextureName = pElem->Attribute ("name");
		const char* pFilePosX = pElem->Attribute ("filePosX");
		const char* pFileNegX = pElem->Attribute ("fileNegX");
		const char* pFilePosY = pElem->Attribute ("filePosY");
		const char* pFileNegY = pElem->Attribute ("fileNegY");
		const char* pFilePosZ = pElem->Attribute ("filePosZ");
		const char* pFileNegZ = pElem->Attribute ("fileNegZ");
		const char *pMipMap = pElem->Attribute("mipmap");

		if (0 == pTextureName) {
			CURITIBA_THROW("Texture has no name in Mat Lib %s", aLib->getName().c_str());
		} 

		if (!(pFilePosX && pFileNegX && pFilePosY && pFileNegY && pFilePosZ && pFileNegZ)) {
			CURITIBA_THROW("CubeMap is not complete or has no label in Mat Lib %s", aLib->getName().c_str());
		}

		sprintf(m_pFullName,"%s::%s", aLib->getName().c_str(), pTextureName);
		if (RESOURCEMANAGER->hasTexture(m_pFullName))
			CURITIBA_THROW("Texture %s is already defined, in Mat Lib %s", m_pFullName, aLib->getName().c_str());

		bool mipmap = true;
		if (pMipMap != 0 && strcmp(pMipMap,"No") == 0)
			mipmap = false;


		std::vector<std::string> files(6);
		files[0] = FileUtil::GetFullPath(path,pFilePosX);
		files[1] = FileUtil::GetFullPath(path,pFileNegX);
		files[2] = FileUtil::GetFullPath(path,pFilePosY);
		files[3] = FileUtil::GetFullPath(path,pFileNegY);
		files[4] = FileUtil::GetFullPath(path,pFilePosZ);
		files[5] = FileUtil::GetFullPath(path,pFileNegZ);

		RESOURCEMANAGER->addTexture (files, m_pFullName, mipmap);		
	}
}


/* -----------------------------------------------------------------------------
STATES

	<states>
		<state name="Grades">
			<alphatest func="GREATER" ref="0.25" />			
			<cull enable=1 face=FRONT|BACK \>
			<depth test=1 write=1 func=LESS />
			<blend src="ONE" dst="ONE" />
			<order value=0 />
		</state>
	</states>

func: NEVER, ALWAYS, LESS, LEQUAL, EQUAL, GEQUAL, GREATER, NOT_EQUAL

order - a number which indicates the order for material drawing: higher values are drawn later,
	negative values are not drawn

	
-----------------------------------------------------------------------------*/
void
ProjectLoader::loadState(TiXmlElement *pElemAux, MaterialLib *aLib, Material *aMat, IState *s) 
{

	TiXmlElement *pElemAux2;

	TiXmlHandle hState (pElemAux);
	pElemAux2 = pElemAux->FirstChildElement ("blend");

	if (pElemAux2) {

		char src[100],dst[100];

		if (TIXML_SUCCESS != pElemAux2->QueryValueAttribute("src", &src) || 
			TIXML_SUCCESS != pElemAux2->QueryValueAttribute ("dst", &dst)){
			CURITIBA_THROW("Blend definition error in library %s", aLib->getName().c_str()); 
		}
		s->enable(IState::BLEND,true);

		/// WARNING: MUST CONVERT STRINGS TO ENUMVALUES ///
		int enumSrc = IState::translateStringToBlendFuncEnum(src);
		int enumDst = IState::translateStringToBlendFuncEnum(dst);
		if (enumSrc != -1 && enumDst != -1) {
			s->setProp( IState::BLEND_SRC, enumSrc);
			s->setProp( IState::BLEND_DST, enumDst);
		}
		else {
			CURITIBA_THROW("Blend definition error in library %s", aLib->getName().c_str());
		}
	}

	pElemAux2 = pElemAux->FirstChildElement ("alphatest");

	if (pElemAux2) {

		float fvalue;

		if (TIXML_SUCCESS != pElemAux2->QueryFloatAttribute("ref", &fvalue)){
			CURITIBA_THROW("AlphaTest definition error in library %s", aLib->getName().c_str());
		}
		s->enable( IState::ALPHA_TEST,true);
		s->setProp( IState::ALPHA_VALUE, fvalue);

		const char* pStateAlphaFunc = pElemAux2->Attribute ("func");
		if (pStateAlphaFunc)
			s->setProp(IState::ALPHA_FUNC, IState::translateStringToFuncEnum(pStateAlphaFunc));
	}

	pElemAux2 = pElemAux->FirstChildElement ("cull");
	if (pElemAux2) {

		int value;
		char face[16];
		int success[2];
		success[0] = !pElemAux2->QueryIntAttribute("enable", &value);
		success[1] = !pElemAux2->QueryValueAttribute("face", &face);
		if (!success[0] && !success[1]){
			CURITIBA_THROW("CullFace definition error in state %s, library %s", s->getName().c_str(),aLib->getName().c_str());
		}
		if (success[0])
			s->enable(IState::CULL_FACE, value!=0);

		if (success[1] ) {
			if (!strcmp(face,"FRONT"))
				s->setProp(IState::CULL_TYPE,IState::FRONT_FACE);
			else
				s->setProp(IState::CULL_TYPE, IState::BACK_FACE);
		}
	}

	pElemAux2 = pElemAux->FirstChildElement ("order");

	if (pElemAux2) {

		int value;
		if (TIXML_SUCCESS != pElemAux2->QueryIntAttribute("value", &value)){
			CURITIBA_THROW("Order definition error in library %s", aLib->getName().c_str());// + " in material: " + aMat->getName());
		}
		s->setProp( IState::ORDER, value);
	}

	pElemAux2 = pElemAux->FirstChildElement("depth");
	if (pElemAux2) {
	
		int value;
		const char *pTest = pElemAux2->Attribute("test");
		if (pTest) {
			if (TIXML_SUCCESS != pElemAux2->QueryIntAttribute("test", &value)){
			CURITIBA_THROW("Depth test definition error in library %s", aLib->getName().c_str());// + " in material: " + aMat->getName());
			}
			s->enable( IState::DEPTH_TEST, value!=0);
		}

		const char *pWrite = pElemAux2->Attribute("write");
		if (pWrite) {
			if (TIXML_SUCCESS != pElemAux2->QueryIntAttribute("write", &value)){
			CURITIBA_THROW("Depth write definition error in library %s", aLib->getName().c_str());// + " in material: " + aMat->getName());
			}
			s->enable( IState::DEPTH_MASK, value!=0);
		}
		const char *pFunc = pElemAux2->Attribute("func");
		if (pFunc) {
			int enumFunc = IState::translateStringToFuncEnum(pFunc);
			if (enumFunc != -1)
				s->setProp(IState::DEPTH_FUNC, enumFunc);
			else {
				CURITIBA_THROW("Depth func definition error in library %s", aLib->getName().c_str());// + " in material: " + aMat->getName());
			}
		}
	}
}


void 
ProjectLoader::loadMatLibStates(TiXmlHandle hRoot, MaterialLib *aLib)
{
	TiXmlElement *pElem;
	pElem = hRoot.FirstChild("states").FirstChild("state").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {
		const char* pStateName = pElem->Attribute ("name");
		
		if (0 == pStateName) {
			CURITIBA_THROW("State has no name in Mat Lib %s", aLib->getName().c_str());
		}

		SLOG("State: %s", pStateName);

		sprintf(m_pFullName, "%s::%s", aLib->getName().c_str(), pStateName);
		if (RESOURCEMANAGER->hasState(m_pFullName))
			CURITIBA_THROW("State %s is already defined, in Mat Lib %s", pStateName,aLib->getName().c_str());


		IState *s = IState::create();

		loadState(pElem,aLib,NULL,s);
		s->setName(m_pFullName);
		RESOURCEMANAGER->addState(s);
	}
}

/* -----------------------------------------------------------------------------
SHADERS

	<shaders>
		<shader name="perpixel-color" ps="../shaders/perpixel-color.frag" vs="../shaders/perpixel-color.vert" />
		...
	</shaders>


	
-----------------------------------------------------------------------------*/
void 
ProjectLoader::loadMatLibShaders(TiXmlHandle hRoot, MaterialLib *aLib, std::string path)
{
	TiXmlElement *pElem;
	pElem = hRoot.FirstChild ("shaders").FirstChild ("shader").Element();
	for ( ; 0 != pElem; pElem=pElem->NextSiblingElement()) {
		const char *pProgramName = pElem->Attribute ("name");

		if (0 == pProgramName) {
			CURITIBA_THROW("Shader has no name in Mat Lib %s", aLib->getName().c_str());
		}

		SLOG("Shader : %s", pProgramName);

		sprintf(m_pFullName, "%s::%s", aLib->getName().c_str(), pProgramName); 
		if (RESOURCEMANAGER->hasProgram(m_pFullName))
			CURITIBA_THROW("Shader %s is already defined, in Mat Lib %s", pProgramName,aLib->getName().c_str());


		const char *pVSFile = pElem->Attribute ("vs");
		const char *pPSFile = pElem->Attribute ("ps");
		const char *pGSFile = pElem->Attribute("gs");

		if (0 == pVSFile || (0 == pPSFile && 0 == pGSFile)) {
			CURITIBA_THROW("Shader %s missing files in Mat Lib %s", pProgramName, aLib->getName().c_str());
		}

		std::string GSFileName;
		std::string FSFileName;

		std::string VSFileName(FileUtil::GetFullPath(path,pVSFile));
		if (!FileUtil::exists(VSFileName))
			CURITIBA_THROW("Shader file %s in MatLib %s does not exist", pVSFile, aLib->getName().c_str());

		if (pGSFile) {
			GSFileName = FileUtil::GetFullPath(path,pGSFile);
			if (!FileUtil::exists(GSFileName))
				CURITIBA_THROW("Shader file %s in MatLib %s does not exist", pGSFile, aLib->getName().c_str());
		}
		if (pPSFile) {
			FSFileName = FileUtil::GetFullPath(path,pPSFile);
			if (!FileUtil::exists(FSFileName))
				CURITIBA_THROW("Shader file %s in MatLib %s does not exist", FSFileName.c_str(), aLib->getName().c_str());
		}

	
		SLOG("Program %s", pProgramName);
		IProgram *aShader = RESOURCEMANAGER->getProgram (m_pFullName);
		aShader->loadShader(IProgram::VERTEX_SHADER, FileUtil::GetFullPath(path,pVSFile));
		SLOG("Shader file %s - %s",pVSFile, aShader->getShaderInfoLog(IProgram::VERTEX_SHADER).c_str());
		if (pPSFile) {
			aShader->loadShader(IProgram::FRAGMENT_SHADER, FileUtil::GetFullPath(path,pPSFile));
			SLOG("Shader file %s - %s",pPSFile, aShader->getShaderInfoLog(IProgram::FRAGMENT_SHADER).c_str());
		}
		if (pGSFile) {
			aShader->loadShader(IProgram::GEOMETRY_SHADER, FileUtil::GetFullPath(path,pGSFile));
			SLOG("Shader file %s - %s",pGSFile, aShader->getShaderInfoLog(IProgram::GEOMETRY_SHADER).c_str());
		}
		aShader->linkProgram();
		SLOG("Linker: %s", aShader->getProgramInfoLog());
	}
}

/* -----------------------------------------------------------------------------
MATERIALCOLOR

	<color>
		<ambient r="0.2" g="0.2" b="0.2" a="1.0" />
		<diffuse r="0.8" g="0.8" b="0.8" a="0.8" />
		<specular r="0.0" g="0.0" b="0.0" a="1.0" />
		<emission r="0.0" g="0.0" b="0.0" a="1.0" />
		<shininess value="0" />
	</color>


	
-----------------------------------------------------------------------------*/
void 
ProjectLoader::loadMaterialColor(TiXmlHandle handle, MaterialLib *aLib, Material *aMat)
{
	TiXmlElement *pElemAux, *pElemAux2;
	pElemAux = handle.FirstChild ("color").Element();
	if (0 != pElemAux) {

		float r, g, b, a;

		pElemAux2 = pElemAux->FirstChildElement ("ambient");
		if (pElemAux2) {
			if (TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("r", &r) || 
				TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("g", &g) ||
				TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("b", &b) ||
				TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("a", &a)){

				CURITIBA_THROW("Color ambient definition error in library %s in material %s", aLib->getName().c_str(), aMat->getName().c_str());
			}
			aMat->getColor().setAmbient (r, g, b, a);
		}

		pElemAux2 = pElemAux->FirstChildElement ("diffuse");
		if (pElemAux2) {
			if (TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("r", &r) || 
				TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("g", &g) ||
				TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("b", &b) ||
				TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("a", &a)){

				CURITIBA_THROW("Color diffuse definition error in library %s in material %s", aLib->getName().c_str(), aMat->getName().c_str());
			}
			aMat->getColor().setDiffuse (r, g, b, a);
		}

		pElemAux2 = pElemAux->FirstChildElement ("specular");
		if (pElemAux2) {
			if (TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("r", &r) || 
				TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("g", &g) ||
				TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("b", &b) ||
				TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("a", &a)){

				CURITIBA_THROW("Color specular definition error in library %s in material %s", aLib->getName().c_str(), aMat->getName().c_str());
			}
			aMat->getColor().setSpecular (r, g, b, a);
		}
	
		pElemAux2 = pElemAux->FirstChildElement ("emission");
		if (pElemAux2) {
			if (TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("r", &r) || 
				TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("g", &g) ||
				TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("b", &b) ||
				TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("a", &a)){

				CURITIBA_THROW("Color emission definition error in library %s in material %s", aLib->getName().c_str(), aMat->getName().c_str());
			}
			aMat->getColor().setEmission (r, g, b, a);		
		}

		float value;
		pElemAux2 = pElemAux->FirstChildElement ("shininess");
		if (pElemAux2) {
			if (TIXML_SUCCESS != pElemAux2->QueryFloatAttribute ("value", &value)){

				CURITIBA_THROW("Color shininess definition error in library %s in material %s", aLib->getName().c_str(), aMat->getName().c_str());
			}
			aMat->getColor().setShininess (value);
		}
	}
}


/* -----------------------------------------------------------------------------
MATERIALTEXTURE

		<textures>
			<texture unit="0" name="shadowMap" >
				<depthCompare mode="COMPARE_REF_TO_TEXTURE" func="LEQUAL" />
				<filtering min="LINEAR" mag="LINEAR" />
			</texture>	
			...
		</textures>

The fields depthCompare and filtering are optional
name refers to a previously defined texture in the same matlib, or a render target in a project file
	
-----------------------------------------------------------------------------*/
void 
ProjectLoader::loadMaterialTextures(TiXmlHandle handle, MaterialLib *aLib, Material *aMat)
{
	TiXmlElement *pElemAux, *pElemAux2;
	pElemAux = handle.FirstChild ("textures").FirstChild ("texture").Element();
	for ( ; 0 != pElemAux; pElemAux = pElemAux->NextSiblingElement()) {
		//const char *pTextureName = pElemAux->GetText();

		const char *pTextureName = pElemAux->Attribute ("name");
		if (0 == pTextureName) {
			CURITIBA_THROW("Texture has no name in library %s in material %s", aLib->getName().c_str(),  aMat->getName().c_str());
		}
		
		int unit;
		if (TIXML_SUCCESS != pElemAux->QueryIntAttribute ("unit", &unit)) {
			CURITIBA_THROW("Texture has no unit in library %s in material %s", aLib->getName().c_str(),  aMat->getName().c_str());
		}

		if (!strchr(pTextureName, ':') )
			sprintf(m_pFullName, "%s::%s", aLib->getName().c_str(), pTextureName);
		else
			sprintf(m_pFullName, "%s", pTextureName);
		if (!RESOURCEMANAGER->hasTexture(m_pFullName))
			CURITIBA_THROW("Texture %s is not defined in library %s in material %s", pTextureName, aLib->getName().c_str(),  aMat->getName().c_str());
			

		aMat->attachTexture (unit, m_pFullName);

		Texture::TextureUnit texUnit = (Texture::TextureUnit)(Texture::TEXTURE0+unit);

		pElemAux2 = pElemAux->FirstChildElement ("depthCompare");
		if (pElemAux2) {
			const char *value = pElemAux2->Attribute("mode");
			if (0 == value){
				CURITIBA_THROW("Depth Compare definition error in library %s in material %s", aLib->getName().c_str(),  aMat->getName().c_str());
			}
			aMat->getTextureSampler(unit)->setTexProp(TextureSampler::TEXTURE_COMPARE_MODE, TextureSampler::translateStringToTexCompareModeEnum(value));
			//aMat->getState()->setTexProp( texUnit,IState::TEXTURE_COMPARE_MODE, IState::translateStringToTexCompareModeEnum(value));

			value = pElemAux2->Attribute("func");
			if (0 == value){
				CURITIBA_THROW("Depth Compare  definition error in library %s in material %s", aLib->getName().c_str(),  aMat->getName().c_str());
			}
			aMat->getTextureSampler(unit)->setTexProp(TextureSampler::TEXTURE_COMPARE_FUNC, TextureSampler::translateStringToTexCompareFuncEnum(value));
			//aMat->getState()->setTexProp( texUnit,IState::TEXTURE_COMPARE_FUNC, IState::translateStringToTexCompareFuncEnum(value));
		}

		pElemAux2 = pElemAux->FirstChildElement ("filtering");
		if (pElemAux2) {
			const char *value = pElemAux2->Attribute("min");
			if (0 == value){
				CURITIBA_THROW("Filtering definition error in library %s in material %s", aLib->getName().c_str(),  aMat->getName().c_str());
			}
			aMat->getTextureSampler(unit)->setTexProp(TextureSampler::TEXTURE_MIN_FILTER, TextureSampler::translateStringToTexMinModeEnum(value));
			//aMat->getState()->setTexProp( texUnit,IState::TEXTURE_MIN_FILTER, IState::translateStringToTexMinModeEnum(value));

			value = pElemAux2->Attribute("mag");
			if (0 == value){
				CURITIBA_THROW("Filtering  definition error in library %s in material %s", aLib->getName().c_str(),  aMat->getName().c_str());
			}
			aMat->getTextureSampler(unit)->setTexProp(TextureSampler::TEXTURE_MAG_FILTER, TextureSampler::translateStringToTexMagModeEnum(value));
			//aMat->getState()->setTexProp( texUnit,IState::TEXTURE_MAG_FILTER, IState::translateStringToTexMagModeEnum(value));
		}
	}
}


/* -----------------------------------------------------------------------------
MATERIALSHADER

	<shader>
		<name>perpixel-color-shadow</name>
		<values>
			<valueof uniform="lightPosition" type="LIGHT" context="Sun" component="POSITION" /> 
		</values>
	</shader>
	
-----------------------------------------------------------------------------*/
void 
ProjectLoader::loadMaterialShader(TiXmlHandle handle, MaterialLib *aLib, Material *aMat)
{
	TiXmlElement *pElemAux, *pElemAux2;

	pElemAux = handle.FirstChild ("shader").Element();
	if (0 != pElemAux) {
		TiXmlHandle hShader (pElemAux);

		pElemAux2 = hShader.FirstChild ("name").Element();
		const char *pShaderName = pElemAux2->GetText();

		if (0 == pShaderName) {
			CURITIBA_THROW("Shader has no target in library %s in material %s", aLib->getName().c_str(), aMat->getName().c_str());
		}
		sprintf(m_pFullName, "%s::%s",aLib->getName().c_str(),pShaderName);
		if (!RESOURCEMANAGER->hasProgram(m_pFullName))
			CURITIBA_THROW("Shader %s is not defined in library %s in material %s", pShaderName,aLib->getName().c_str(), aMat->getName().c_str());

		
		aMat->attachProgram (m_pFullName);

		pElemAux2 = hShader.FirstChild ("values").FirstChild ("valueof").Element();
		for ( ; 0 != pElemAux2; pElemAux2 = pElemAux2->NextSiblingElement()) {
			const char *pUniformName = pElemAux2->Attribute ("uniform");
			const char *pComponent = pElemAux2->Attribute ("component");
			const char *pContext = pElemAux2->Attribute("context");
			const char *pType = pElemAux2->Attribute("type");
			//const char *pId = pElemAux2->Attribute("id");

			if (0 == pUniformName) {
				CURITIBA_THROW("No uniform name, in library %s in material %s", aLib->getName().c_str(), aMat->getName().c_str());
			}
			if (0 == pType) {
				CURITIBA_THROW("No type found for uniform %s, in library %s in material %s", pUniformName, aLib->getName().c_str(), aMat->getName().c_str());
			}
			if (0 == pContext) {
				CURITIBA_THROW("No context found for uniform %s, in library %s in material %s", pUniformName, aLib->getName().c_str(), aMat->getName().c_str());
			}
			if (0 == pComponent) {
				CURITIBA_THROW("No component found for uniform %s, in library %s in material %s", pUniformName, aLib->getName().c_str(), aMat->getName().c_str());
			}
			if (!ProgramValue::Validate(pType, pContext, pComponent))
				CURITIBA_THROW("Uniform %s is not valid, in library %s in material %s", pUniformName, aLib->getName().c_str(), aMat->getName().c_str());

			int id = 0;
			if (((strcmp(pContext,"LIGHT") == 0) || (0 == strcmp(pContext,"TEXTURE"))) &&  (0 != strcmp(pComponent,"COUNT"))) {
				if (TIXML_SUCCESS != pElemAux2->QueryIntAttribute ("id", &id))
					CURITIBA_THROW("No id found for uniform %s, in library %s in material %s", pUniformName, aLib->getName().c_str(), aMat->getName().c_str());
				if (id < 0)
					CURITIBA_THROW("id must be non negative, in uniform %s, in library %s in material %s", pUniformName, aLib->getName().c_str(), aMat->getName().c_str());
			}
			std::string s(pType);
			if (s == "TEXTURE") {
				sprintf(m_pFullName, "%s::%s", aLib->getName().c_str(),pContext);
				if (!RESOURCEMANAGER->hasTexture(m_pFullName)) {
					CURITIBA_THROW("Texture %s is not defined, in library %s in material %s", m_pFullName, aLib->getName().c_str(), aMat->getName().c_str());
				}
				else
					aMat->addProgramValue (pUniformName, ProgramValue (pUniformName,pType, m_pFullName, pComponent, id));
			}

			else if (s == "CAMERA") {
				// Must consider that a camera can be defined internally in a pass, example:lightcams
				/*if (!RENDERMANAGER->hasCamera(pContext))
					CURITIBA_THROW("Camera %s is not defined in the project file", pContext);*/
				aMat->addProgramValue (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));
			}
			else if (s == "LIGHT") {
				if (!RENDERMANAGER->hasLight(pContext))
					CURITIBA_THROW("Light %s is not defined in the project file", pContext);
				aMat->addProgramValue (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));

			}
			else
				aMat->addProgramValue (pUniformName, ProgramValue (pUniformName,pType, pContext, pComponent, id));

				//sprintf(m_pFullName, "%s(%s,%s)",pType,pContext,pComponent );

			
		}
	}
}

/* -----------------------------------------------------------------------------
MATERIALSTATE

	<state>
		<alphatest alphaFunc="GREATER" alphaRef="0.25" />
		<blend src="ONE" dst="ZERO" />
		<cull value="0" />
		<order value="1" />
	</state>	

alphaFunc: ALPHA_NEVER, ALPHA_ALWAYS, ALPHA_LESS, ALPHA_LEQUAL,
				ALPHA_EQUAL, ALPHA_GEQUAL, ALPHA_GREATER, ALPHA_NOT_EQUAL

order: is a value that defines the order of rendering. Higher values are drawn later.

src and dst: ZERO,ONE,SRC_COLOR,ONE_MINUS_SRC_COLOR,DST_COLOR,
				ONE_MINUS_DST_COLOR, SRC_ALPHA, ONE_MINUS_SRC_ALPHA, DST_ALPHA,
				ONE_MINUS_DST_ALPHA, SRC_ALPHA_SATURATE, CONSTANT_COLOR, 
				ONE_MINUS_CONSTANT_COLOR, CONSTANT_ALPHA, ONE_MINUS_CONSTANT_ALPHA

	OR

	<state name="bla" />

where bla is previously defined in the mat lib.
	

-----------------------------------------------------------------------------*/

void 
ProjectLoader::loadMaterialState(TiXmlHandle handle, MaterialLib *aLib, Material *aMat)
{
	TiXmlElement *pElemAux;

	pElemAux = handle.FirstChild("state").Element();
	if (0 != pElemAux) {
		const char *pStateName = pElemAux->Attribute ("name");
		//definition by ref
		if (0 != pStateName) {

			sprintf(m_pFullName, "%s::%s", aLib->getName().c_str(), pStateName);
			if (!RESOURCEMANAGER->hasState(m_pFullName))
				CURITIBA_THROW("State %s is not defined in library %s, in material %s", pStateName,aLib->getName().c_str(), aMat->getName().c_str());


			aMat->setState(RESOURCEMANAGER->getState(m_pFullName));
		}
		else { // definition inline
			loadState(pElemAux,aLib,aMat,aMat->getState());
		}
	}

}

/* -----------------------------------------------------------------------------
MATERIAL LIBS     

	<?xml version="1.0" ?>
	<materiallib name="CubeMap">
		<shaders>
			... // see loadMatLibShaders
		</shaders>
		<textures>
			... // see loadMatLibTextures
		</textures>
		<states>
			... // see loadMatLibStates
		</states>
		<materials>
			<material name = "bla">
				<color>
					... see loadMaterialColor
				</color>
				<textures>
					... see loadMaterialTextures
				</textures>
				<state>
					... see loadMaterialState
				</state>
				<shader>
					... see loadMaterialShader
				</shader>
			</material>
		</materials>
	...
	</materiallib>


-----------------------------------------------------------------------------*/

void 
ProjectLoader::loadMatLib (std::string file)
{
	std::string path = FileUtil::GetPath(file);
	//std::map<std::string,IState *> states;

	TiXmlDocument doc (file.c_str());
	bool loadOkay = doc.LoadFile();

	MaterialLib *aLib = 0;

	if (!loadOkay) 
		CURITIBA_THROW("Parsing Error -%s- Line(%d) Column(%d) in file: %s", doc.ErrorDesc(), doc.ErrorRow(), doc.ErrorCol(),file.c_str());

	TiXmlHandle hDoc (&doc);
	TiXmlHandle hRoot (0);
	TiXmlElement *pElem;

	{ //root
		pElem = hDoc.FirstChildElement().Element();
		if (0 == pElem) 
			CURITIBA_THROW("Parse problem in mat lib file %s", file.c_str());
		hRoot = TiXmlHandle (pElem);
	}

	pElem = hRoot.Element();
	const char* pName = pElem->Attribute ("name");

	if (0 == pName) 
		CURITIBA_THROW("Material lib has no name in file %s", file.c_str());

	SLOG("Material Lib Name: %s", pName);

	aLib = MATERIALLIBMANAGER->getLib (pName);

	loadMatLibRenderTargets(hRoot, aLib, path);
	loadMatLibTextures(hRoot, aLib,path);
	loadMatLibStates(hRoot, aLib);
	loadMatLibShaders(hRoot, aLib, path);

	pElem = hRoot.FirstChild ("materials").FirstChild ("material").Element();
	for ( ; 0 != pElem; pElem = pElem->NextSiblingElement()) {

		TiXmlHandle handle (pElem);

		const char *pMaterialName = pElem->Attribute ("name");

		if (0 == pMaterialName) 
			CURITIBA_THROW("Material has no name in material lib %s", pName);

		if (MATERIALLIBMANAGER->hasMaterial(pName, pMaterialName))
			CURITIBA_THROW("Material %s is already defined in library %s", pMaterialName,pName);

		SLOG("Material: %s", pMaterialName);

		//Material *mat = new Material;
		//mat->setName (pMaterialName);
		Material *mat = MATERIALLIBMANAGER->createMaterial(pName,pMaterialName);

		loadMaterialColor(handle,aLib,mat);
		loadMaterialTextures(handle,aLib,mat);
		loadMaterialShader(handle,aLib,mat);
		loadMaterialState(handle,aLib,mat);

		//aLib->addMaterial (mat);
	}
}
