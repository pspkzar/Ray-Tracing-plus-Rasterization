#ifndef MATERIALLIBMANAGER_H
#define MATERIALLIBMANAGER_H

#include <string>
#include <map>

#include <curitiba/material/materialid.h>
#include <curitiba/material/materiallib.h>

using namespace curitiba::material;

// FIXME: Don't know where this should be defined. Added it here for now
#define DEFAULTMATERIALLIBNAME "curitiba_material_lib"

namespace curitiba
{
	namespace material
	{

		class MaterialLibManager {

		private:
			std::map<std::string, curitiba::material::MaterialLib*> m_LibManager;
			MaterialLib *m_DefaultLib;


		public:
			MaterialLibManager();
			~MaterialLibManager();

			void clear();

			bool hasLibrary (std::string lib);
			curitiba::material::MaterialLib* getLib(std::string libName);

			bool hasMaterial (std::string aLibrary, std::string name);
			void addMaterial (std::string aLibrary, curitiba::material::Material* aMaterial);
			Material* getDefaultMaterial (std::string name);
			Material* getMaterial (curitiba::material::MaterialID &materialID);
			Material* getMaterial (std::string lib, std::string material);

			Material* createMaterial(std::string lib, std::string material);
			Material* createMaterial(std::string material);


			std::vector<std::string>* getLibNames ();
			std::vector<std::string>* getMaterialNames (const std::string &lib);
			
			int getNumLibs();

		private:
			MaterialLibManager(const MaterialLibManager&);
			MaterialLibManager& operator= (const MaterialLibManager&);

			// adds some materials 
			//void addOwnMaterials();
		};
	};
};

#endif
